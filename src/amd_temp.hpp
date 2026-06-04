#pragma once

namespace AmdTemp {
	// Initialize ADL, returns true on success
	bool init();

	// Read CPU temperature via ADL PMLOG. Returns temp in °C, or -1 on failure.
	int readCpuTemp();

	// Cleanup
	void cleanup();
}
