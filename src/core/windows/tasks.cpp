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

static bool couninitialize_released_tasks = false;

std::wostream& tasks() noexcept{
	BSTR task_name;

	ITaskService *service_pointer = nullptr;
	ITaskFolder *root_folder_pointer = nullptr;
	ITaskFolderCollection *folders_pointer = nullptr;
	IRegisteredTaskCollection *tasks_pointer = nullptr;
	IRegisteredTask *task_ptr = nullptr;

	LONG tasks_count = 0, i = 0;

	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) return std::wcerr << i18n_system::ERROR_TASKS
		<< " COINITIALIZEEX" << std::endl << std::endl;

	std::unique_ptr<void, decltype([](void*){
		if(couninitialize_released_tasks) return;
		couninitialize_released_tasks = true;
		CoUninitialize();
	})> couninit_ptr(reinterpret_cast<void*>(1));

	if(FAILED(CoCreateInstance(CLSID_TaskScheduler, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskService,
		reinterpret_cast<void**>(&service_pointer)))) return std::wcerr << i18n_system::ERROR_TASKS
		<< " COCREATEINSTANCE" << std::endl << std::endl;

	std::unique_ptr<ITaskService, decltype([](ITaskService *ptr){
		if(ptr) ptr->Release();
	})> service_ptr_releaser(service_pointer);

	if(FAILED(service_pointer->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t())))
		return std::wcerr << i18n_system::ERROR_TASKS << " ITASKSERVICE" << std::endl << std::endl;

	if(FAILED(service_pointer->GetFolder(_bstr_t(L"\\"), &root_folder_pointer)))
		return std::wcerr << i18n_system::ERROR_TASKS << " GetFolder" << std::endl << std::endl;

	std::unique_ptr<ITaskFolder, decltype([](ITaskFolder *ptr){
		if(ptr) ptr->Release();
	})> root_folder_ptr_releaser(root_folder_pointer);

	if(FAILED(root_folder_pointer->GetFolders(0, &folders_pointer)))
		return std::wcerr << i18n_system::ERROR_TASKS << " GetFolders" << std::endl << std::endl;

	std::unique_ptr<ITaskFolderCollection, decltype([](ITaskFolderCollection *ptr){
		if(ptr) ptr->Release();
	})> folders_ptr_release(folders_pointer);


	if(FAILED(root_folder_pointer->GetTasks(TASK_ENUM_HIDDEN, &tasks_pointer)))
		return std::wcerr << i18n_system::ERROR_TASKS << " GetTasks" << std::endl << std::endl;

	std::unique_ptr<IRegisteredTaskCollection, decltype([](IRegisteredTaskCollection *ptr){
		if(ptr) ptr->Release();
	})> tasks_ptr_releaser(tasks_pointer);

	std::wcout << i18n::TASKS << std::endl;

	tasks_pointer->get_Count(&tasks_count);

	for(; i <= tasks_count; ++i){
		if(FAILED(tasks_pointer->get_Item(_variant_t(i), &task_ptr))) continue;

		std::unique_ptr<IRegisteredTask, decltype([](IRegisteredTask *ptr){
			if(ptr) ptr->Release();
		})> tasks_ptr_rel(task_ptr);

		if(FAILED(task_ptr->get_Name(&task_name))) continue;

		std::unique_ptr<wchar_t, decltype([](BSTR bstr){
			if(bstr) SysFreeString(bstr);
		})> bstr_ptr(task_name);

		std::wcout << L'\t' << task_name << std::endl;
	}

	return std::wcout << std::endl;
}
