// Fill out your copyright notice in the Description page of Project Settings.

#include "FTPHandler.h"
#include "DesktopPlatformModule.h"

void UFTPHandler::BeginDestroy()
{
	ftp.Quit();
	Super::BeginDestroy();
}
void UFTPHandler::Tick(float DeltaTime)
{
	if (DownState != -1)
	{	
		DownloadCB.Broadcast(DownState == 1);
		DownState = -1;
	}
	if (UpState != -1)
	{
		UploadCB.Broadcast(UpState == 1);
		UpState = -1;
	}

}

bool UFTPHandler::IsTickable() const
{
	return true;
}
TStatId UFTPHandler::GetStatId() const
{
	return Super::GetStatID();
}
bool UFTPHandler::Connect(const FString& ip, int32 port, const FString& name, const FString& pwd)
{
	if (!ftp.Connect(TCHAR_TO_UTF8(*(ip + TEXT(":") + FString::FromInt(port)))))
	{
		return false;
	}
	if (!ftp.Login(TCHAR_TO_UTF8(*name), TCHAR_TO_UTF8(*pwd)))
	{
		return false;
	}
	return true;
}

bool UFTPHandler::MakeDir(const FString& DirName)
{
	return ftp.Mkdir(TCHAR_TO_UTF8(*DirName)) == 1;
}

bool UFTPHandler::RemoveDir(const FString& DirName)
{
	return ftp.Rmdir(TCHAR_TO_UTF8(*DirName)) == 1;
}

void UFTPHandler::Download(const FString& localFilePath, const FString& RemoteFilePath)
{
	localPath = localFilePath;
	RemotePath = RemoteFilePath;
	AsyncTask(ENamedThreads::AnyThread, [=]()
		{
			DownState = ftp.Get(TCHAR_TO_UTF8(*localPath), TCHAR_TO_UTF8(*RemotePath), ftplib::image);
			
		});

	
}

void UFTPHandler::WindowUpload()
{
	TArray<FString> OpenFileNames;//获取的文件绝对路径
	FString ExtensionStr = TEXT("*.*");//文件类型

	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	DesktopPlatform->OpenFileDialog(nullptr, TEXT("ywy文件管理器"), FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()), TEXT(""), *ExtensionStr, EFileDialogFlags::None, OpenFileNames);
	if (OpenFileNames.Num() != 0)
	{
		FString FileName = FPaths::GetCleanFilename(OpenFileNames[0]);
		Upload(OpenFileNames[0], FileName);
	}
}
void UFTPHandler::Upload(const FString& filePath, const FString& RemoteFilePath)
{
	localPath = filePath;
	RemotePath = RemoteFilePath;
	AsyncTask(ENamedThreads::AnyThread, [&]()
		{
			UpState = ftp.Put(TCHAR_TO_UTF8(*localPath), TCHAR_TO_UTF8(*RemotePath), ftplib::image);
			
		});

}

bool UFTPHandler::RemoveFile(const FString& RemoteFilePath)
{
	return ftp.Delete(TCHAR_TO_UTF8(*RemoteFilePath)) == 1;
}


