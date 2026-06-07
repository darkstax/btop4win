## v1.0.8 (darkstax fork)

* 开始本地化进程（优先中文），新增 CJK 感知对齐基础设施
* Added: Chinese (zh) localization — 13+ buttons, memory labels, sort options, GPU→显存
* Added: `rjustW`/`ljustW`/`cjustW` CJK-aware alignment functions using `langDisplayWidth()`
* Added: `langSortName()` mapping for process sort option display names
* Fixed: UTF-8 CJK character truncation (substr → uresize with wide=true)
* Fixed: Memory box label alignment using dynamic display-width calculation
* Fixed: Process detail box column headers centered with `cjustW`
* Fixed: Process list headers aligned with `ljustW`/`rjustW`
* Fixed: Button hotkey fragments updated to complete words in lang_en.cpp
* Changed: Button rendering uses `L->btn_*` for full localization support

## v1.0.7 (darkstax fork)

* Added: AMD ADL PMLOG direct CPU temperature reading (sensor 504)
* Added: HWiNFO shared memory as universal fallback temperature source
* Added: CPUID vendor detection for automatic temp source priority
* Changed: AMD uses ADL first, Intel uses PawnIO/MSR first
* Changed: UAC level from RequireAdministrator to AsInvoker (PawnIO backend)
* Changed: Updated config/menu help text for PawnIO driver requirement
* Fixed: ADLPMLogDataOutput struct layout (was reading wrong memory offsets)
* Fixed: ADL sensor ID (504=CPU, not 500=Edge which is unsupported on APUs)
* Fixed: AmdTemp::cleanup() now called on exit

## v1.0.6 (darkstax fork)

* Added: Initial AMD ADL temperature module
* Changed: LibreHardwareMonitor backend to PawnIO

## v1.0.5
* Updated: LibreHardwareMonitor to latest version (0.9.4)
* Added: Better detection of GPU total memory
* Added: Add option for narrow and wide cpu views, @samiksome92
* Fixed: Addressed potential race condition in exit handler, by @bot984397

## v1.0.4

* Fixed: Gpu total memory will now default to highest recorded usage if not detected.
* Added: Config option to override gpu total memory.
* Added: New theme paper, by @s6muel

## v1.0.3

* Fixed: Config not saving when quittting by closing with window controls
* Changed: Ignore warning for WMI CoInitializeSecurity() when it's already have been called
* Fixed: Core mapping for cpu temps
* Fixed: `io_graph_speeds` config setting

## v1.0.2

* Fixed: Tools::hostname() and Tools::username()
* Fixed: Net graphs sync rescaling
* Fixed: Memory values not clearing properly when not in graph mode in mem box
* Changed: Stalls only force quit if over 1 minute, (disks waking up seems to be causing short stalls)
* Fixed: Cpu clock for Ryzen Mobile

## v1.0.1

* Changed: Switched from using OpenHardwareMonitor to LibreHardwareMonitor DLL import
* Fixed: CPU/GPU temps for Ryzen mobile CPUs
* Fixed: More fixes for Ryzen mobile

## v1.0.0

* First release
