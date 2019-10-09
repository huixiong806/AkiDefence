#pragma once
#include <vector>
#include <string>
#include <regex>
#include <io.h>
#include <Windows.h>
#include <shellapi.h>
#include <tchar.h>
class FileManager
{
public:
	static std::vector<std::string> getFolder(std::string path)
	{
		std::vector<std::string> result;
		//文件句柄  
		long hFile = 0;
		//文件信息  
		struct _finddata_t fileinfo;
		std::string p;
		if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
		{
			do
			{
				if (((fileinfo.attrib &  _A_SUBDIR)&& fileinfo.name[0]!='.'))
					result.push_back(p.assign(path).append("/").append(fileinfo.name));
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
		return result;
	}
	static std::vector<std::string> getAllFiles(std::string path, std::string suffix)
	{
		std::vector<std::string> files;
		//文件句柄
		intptr_t   hFile = 0;
		//文件信息
		struct _finddata_t fileinfo;
		std::string pathName, exdName;
		if (0 != strcmp(suffix.c_str(), ""))
		{
			exdName = "\\*." + suffix;
		}
		else
		{
			exdName = "\\*";
		}

		if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
		{
			do
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					//files.push_back(pathName.assign(path).append("\\").append(fileinfo.name)); // 要得到绝对目录使用该语句
					//如果使用
					files.push_back(fileinfo.name); // 只要得到文件名字使用该语句
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
		return files;
	}
	// 删除文件
	static bool deleteFile(std::string filename)
	{
		if (-1 == remove(filename.c_str()))
		{
			return false;
		}
		return true;
	}
	//运行一个程序，并返回是否超时(毫秒)
	static bool openEXE(std::string filename,size_t time)
	{
		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpParameters = NULL;
		ShExecInfo.lpVerb = _T("open");
		std::string AIpath = filename;
		ShExecInfo.lpFile = (LPCSTR)AIpath.c_str();
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		DWORD result=WaitForSingleObject(ShExecInfo.hProcess, time);
		if (result == WAIT_TIMEOUT)
		{
			TerminateProcess(ShExecInfo.hProcess, 0);
			return true;
		}
		else return false;
	}
};