// broker_thread.cpp — Broker 后台线程实现
//
// 设计原则:
// 1. 启动时检查 exe 是否存在 → 不存在永久跳过
// 2. COM connect 在独立线程 → 永不阻塞 runner
// 3. 数据写入 g_brokerCache → runner 线程只读
// 4. 失败 >5 次自动放弃

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <thread>
#include <chrono>

#include "broker_thread.hpp"
#include "broker_client.hpp"

using std::this_thread::sleep_for;

bool BrokerThread::brokerExeExists() {
	const wchar_t* candidates[] = {
		L"%LOCALAPPDATA%\\SysMonCmdPal\\broker-staging\\SysMonBroker.exe",
		L"%LOCALAPPDATA%\\SysMonCmdPal\\SysMonBroker.exe",
		L"%PROGRAMFILES%\\SysMonBroker\\SysMonBroker.exe",
	};
	for (auto& path : candidates) {
		wchar_t expanded[MAX_PATH];
		ExpandEnvironmentStringsW(path, expanded, MAX_PATH);
		if (GetFileAttributesW(expanded) != INVALID_FILE_ATTRIBUTES)
			return true;
	}
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
	// COM init（这个线程只在这里 init 一次）
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
					// 认证成功 → 标记 broker 可用
					g_brokerCache.broker_available = true;
					// 检测 .devmode
					wchar_t devPath[MAX_PATH];
					ExpandEnvironmentStringsW(L"%LOCALAPPDATA%\\SysMonCmdPal\\.devmode", devPath, MAX_PATH);
					g_brokerCache.devmode = (GetFileAttributesW(devPath) != INVALID_FILE_ATTRIBUTES);
				}
			} else {
				failCount++;
			}
		}

		// 已连接且已认证 → 拉取数据
		if (client.isConnected() && client.isAuthenticated()) {
			try {
				// --- 进程数据 ---
				auto procs = client.getProcesses();
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

				// --- 传感器数据 ---
				{
					std::lock_guard lck(g_brokerCache.mtx);
					g_brokerCache.cpu_temp     = client.getCpuTemperature();
					g_brokerCache.gpu_temp     = client.getGpuTemperature(0);
					g_brokerCache.gpu_usage    = client.getGpuUsage(0);
					// GPU 名称只在第一次设置
					if (g_brokerCache.gpu_name.empty()) {
						g_brokerCache.gpu_name = client.getGpuName(0);
					}
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
