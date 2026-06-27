// broker_thread.cpp — Broker 后台线程实现

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <thread>
#include <chrono>

#include "broker_thread.hpp"
#include "broker_client.hpp"
#include "btop_tools.hpp"

using std::this_thread::sleep_for;

bool BrokerThread::brokerExeExists() {
	const wchar_t* candidates[] = {
		L"%LOCALAPPDATA%\\SysMonBroker\\SysMonBroker.exe",
		L"%LOCALAPPDATA%\\SysMonCmdPal\\SysMonBroker.exe",
		L"%LOCALAPPDATA%\\SysMonCmdPal\\broker-staging\\SysMonBroker.exe",
		L"%PROGRAMFILES%\\SysMonBroker\\SysMonBroker.exe",
	};
	for (auto& path : candidates) {
		wchar_t expanded[MAX_PATH];
		ExpandEnvironmentStringsW(path, expanded, MAX_PATH);
		if (GetFileAttributesW(expanded) != INVALID_FILE_ATTRIBUTES) {
			return true;
		}
	}
	Logger::debug("Broker: exe not found, permanently disabled");
	return false;
}

void BrokerThread::start() {
	if (!brokerExeExists()) {
		s_unavailable = true;
		return;
	}

	if (s_running) return;
	s_running = true;
	s_thread = std::thread(run);
}

void BrokerThread::stop() {
	s_running = false;
	if (s_thread.joinable()) s_thread.join();
}

void BrokerThread::run() {

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	bool comOk = SUCCEEDED(hr);
	if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) {
		s_unavailable = true;
		return;
	}

	BrokerClient client;
	int failCount = 0;

	while (s_running) {
		if (!client.isConnected()) {
			if (client.connect()) {
				failCount = 0;

				int authResult = client.authenticate();
				if (authResult != 0) {
					client.disconnect();
					failCount++;
				} else {
					g_brokerCache.broker_available = true;
				}
			} else {
				failCount++;
			}
		}

		if (client.isConnected() && client.isAuthenticated()) {
			try {
				auto procs = client.getProcesses();
				double cpuTemp = client.getCpuTemperature();
				double cpuClock = client.getCpuClock();

				if (!procs.empty()) {
					std::lock_guard lck(g_brokerCache.mtx);
					g_brokerCache.proc_list.clear();
					g_brokerCache.proc_list.reserve(procs.size());
					for (auto& bp : procs) {
						BrokerProcData pd;
						pd.pid     = bp.pid;
						pd.ppid    = bp.ppid;
						pd.threads = bp.threads;
						pd.name    = std::move(bp.name);
						pd.cmd     = std::move(bp.cmd);
						pd.user    = std::move(bp.user);
						pd.mem     = bp.mem;
						pd.cpu_p   = bp.cpu_p;
						pd.cpu_s   = bp.cpu_s;
						pd.cpu_t   = bp.cpu_t;
						g_brokerCache.proc_list.push_back(std::move(pd));
					}
					g_brokerCache.proc_fresh = true;
				}

				{
					std::lock_guard lck(g_brokerCache.mtx);
					g_brokerCache.cpu_temp     = cpuTemp;
					g_brokerCache.cpu_clock    = cpuClock;
					g_brokerCache.sensor_fresh    = true;
					g_brokerCache.broker_available = true;
				}

				sleep_for(std::chrono::milliseconds(2000));

			} catch (...) {
				client.disconnect();
			}
		} else {
			failCount++;
			if (failCount > 5) {
				s_unavailable = true;
				break;
			}
			sleep_for(std::chrono::milliseconds(3000));
		}
	}

	client.disconnect();
	if (comOk) CoUninitialize();
}
