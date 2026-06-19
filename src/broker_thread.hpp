// broker_thread.hpp — 独立 Broker 后台线程 + 共享缓存
// Broker 线程完全独立运行，runner 线程只从缓存读取，永不阻塞
#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>

// 从 Broker 获取的简化进程信息（比 BrokerProcessEntry 轻量，方便拷贝）
struct BrokerProcData {
	size_t pid = 0;
	size_t ppid = 0;
	size_t threads = 0;
	std::string name;
	std::string cmd;
	std::string user;
	uint64_t mem = 0;
	double cpu_p = 0.0;
	uint64_t cpu_s = 0;  // creation time (FILETIME)
	uint64_t cpu_t = 0;  // kernel + user time
};

// 共享缓存 — runner 线程只读，Broker 线程写
struct BrokerCache {
	mutable std::mutex mtx;

	// 进程数据
	std::vector<BrokerProcData> proc_list;
	bool proc_fresh = false;

	// 传感器数据
	double cpu_temp = -1;
	double gpu_temp = -1;
	double gpu_usage = -1;
	std::string gpu_name;
	bool sensor_fresh = false;

	// 整体状态
	bool broker_available = false;
	bool devmode = false;   // .devmode 文件存在且 broker 可用
};

// 全局缓存单例
inline BrokerCache g_brokerCache;

class BrokerThread {
public:
	// 启动后台线程。内部检查 Broker exe 是否存在，不存在则永久跳过。
	static void start();

	// 停止后台线程（进程退出时调用）
	static void stop();

private:
	static void run();
	static bool brokerExeExists();

	inline static std::thread s_thread;
	inline static std::atomic<bool> s_running{false};
	inline static std::atomic<bool> s_unavailable{false};
};
