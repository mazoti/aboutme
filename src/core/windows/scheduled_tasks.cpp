module;

#include <iostream>
#include <memory>

#include <Windows.h>
#include <taskschd.h>
#include <comdef.h>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "taskschd.lib")

module core;

import i18n;
import i18n_system;

// Defines a custom deleter for smart pointers managing COM-like objects
template <typename T> struct releaser{ void operator()(T* ptr) const{if(ptr){ptr->Release();}}};

// Lists scheduled tasks from the Windows Task Scheduler
std::wostream& scheduled_tasks() noexcept{
	BSTR task_name;

	IRegisteredTask           *task_ptr            = nullptr;
	IRegisteredTaskCollection *tasks_pointer       = nullptr;

	ITaskService              *service_pointer     = nullptr;
	ITaskFolder               *root_folder_pointer = nullptr;

	LONG tasks_count = 0, i = 1;

	// Initializes COM library in multithreaded mode
	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) return std::wcerr << i18n_system::ERROR_TASKS
		<< L" COINITIALIZEEX\n\n";
	std::unique_ptr<void, decltype([](void*){ CoUninitialize(); })> couninit_ptr(reinterpret_cast<void*>(1));

	// Creates an instance of the Task Scheduler service
	if(FAILED(CoCreateInstance(CLSID_TaskScheduler, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskService,
		reinterpret_cast<void**>(&service_pointer)))) return std::wcerr << i18n_system::ERROR_TASKS
			<< L" COCREATEINSTANCE\n\n";
	std::unique_ptr<ITaskService, releaser<ITaskService>> service_ptr_releaser(service_pointer);

	// Connects to the Task Scheduler service
	if(FAILED(service_pointer->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t())))
		return std::wcerr << i18n_system::ERROR_TASKS << L" ITASKSERVICE\n\n";

	if(FAILED(service_pointer->GetFolder(_bstr_t(L"\\"), &root_folder_pointer)))
		return std::wcerr << i18n_system::ERROR_TASKS << L" GetFolder\n\n";
	std::unique_ptr<ITaskFolder, releaser<ITaskFolder>> root_folder_ptr_releaser(root_folder_pointer);

	// Gets all tasks in the root folder, including hidden ones
	if(FAILED(root_folder_pointer->GetTasks(TASK_ENUM_HIDDEN, &tasks_pointer)))
		return std::wcerr << i18n_system::ERROR_TASKS << L" GetTasks\n\n";

	std::unique_ptr<IRegisteredTaskCollection, releaser<IRegisteredTaskCollection>> tasks_ptr_releaser(tasks_pointer);

	tasks_pointer->get_Count(&tasks_count);
	if(!tasks_count) return std::wcout;

	std::wcout << i18n::SCHEDULED_TASKS << L'\n';

	// Iterates through all tasks
	for(; i <= tasks_count; ++i){
		if(FAILED(tasks_pointer->get_Item(_variant_t(i), &task_ptr))) continue;
		std::unique_ptr<IRegisteredTask, releaser<IRegisteredTask>> tasks_ptr_rel(task_ptr);

		// Gets the name of the task
		if(FAILED(task_ptr->get_Name(&task_name))) continue;
		std::unique_ptr<wchar_t, decltype([](BSTR bstr){ if(bstr) SysFreeString(bstr);})> bstr_ptr(task_name);

		std::wcout << L'\t' << task_name << L'\n';
	}

	return std::wcout << L'\n';
}
