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

struct co_uninitialize{ void operator()(void*) const{ CoUninitialize(); }};

struct sys_free_string{ void operator()(BSTR bstr) const{ if(bstr) SysFreeString(bstr); }};

template <typename T> struct releaser{ void operator()(T* ptr) const{ if(ptr) ptr->Release(); }};

std::wostream& tasks() noexcept{
	BSTR task_name;

	ITaskService *service_pointer = nullptr;
	ITaskFolder *root_folder_pointer = nullptr;
	ITaskFolderCollection *folders_pointer = nullptr;
	IRegisteredTaskCollection *tasks_pointer = nullptr;
	IRegisteredTask* task_ptr = nullptr;

	LONG tasks_count = 0, i = 0;

	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) return std::wcerr << ERROR_TASKS
		<< " COINITIALIZEEX" << std::endl << std::endl;
	std::unique_ptr<void, co_uninitialize> couninit_ptr(reinterpret_cast<void*>(1), co_uninitialize());

	if(FAILED(CoCreateInstance(CLSID_TaskScheduler, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskService,
		reinterpret_cast<void**>(&service_pointer)))) return std::wcerr << ERROR_TASKS
		<< " COCREATEINSTANCE" << std::endl << std::endl;
	std::unique_ptr<ITaskService, releaser<ITaskService> > service_ptr_releaser(service_pointer);

	if(FAILED(service_pointer->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t())))
		return std::wcerr << ERROR_TASKS << " ITASKSERVICE" << std::endl << std::endl;

	if(FAILED(service_pointer->GetFolder(_bstr_t(L"\\"), &root_folder_pointer)))
		return std::wcerr << ERROR_TASKS << " GetFolder" << std::endl << std::endl;
	std::unique_ptr<ITaskFolder, releaser<ITaskFolder> > root_folder_ptr_releaser(root_folder_pointer);

	if(FAILED(root_folder_pointer->GetFolders(0, &folders_pointer)))
		return std::wcerr << ERROR_TASKS << " GetFolders" << std::endl << std::endl;
	std::unique_ptr<ITaskFolderCollection, releaser<ITaskFolderCollection> > folders_ptr_release(folders_pointer);

	if(FAILED(root_folder_pointer->GetTasks(TASK_ENUM_HIDDEN, &tasks_pointer)))
		return std::wcerr << ERROR_TASKS << " GetTasks" << std::endl << std::endl;
	std::unique_ptr<IRegisteredTaskCollection, releaser<IRegisteredTaskCollection> > tasks_ptr_releaser(tasks_pointer);


	std::wcout << TASKS << std::endl;

	tasks_pointer->get_Count(&tasks_count);

	for(; i <= tasks_count; ++i){
		if(FAILED(tasks_pointer->get_Item(_variant_t(i), &task_ptr))) continue;
		std::unique_ptr<IRegisteredTask, releaser<IRegisteredTask> > tasks_ptr_rel(task_ptr);

		if(FAILED(task_ptr->get_Name(&task_name))) continue;
		std::unique_ptr<wchar_t, sys_free_string> bstr_ptr(task_name);

		std::wcout << L'\t' << task_name << std::endl;
	}

	return std::wcout << std::endl;
}
