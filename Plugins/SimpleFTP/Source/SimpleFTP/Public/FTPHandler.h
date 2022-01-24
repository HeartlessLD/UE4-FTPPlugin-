// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ftplib.h"

#include "FTPHandler.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFTPDownloadDelegate, bool, success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFTPUploadDelegate, bool, success);
/**
 * 
 */
UCLASS(BlueprintType)
class SIMPLEFTP_API UFTPHandler : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:

	virtual void BeginDestroy() override;
	// Begin FTickableGameObject Interface.
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	// End FTickableGameObject Interface.


	UFUNCTION(BlueprintCallable)
		bool Connect(const FString& ip,  int32 port, const FString& name, const FString& pwd);
	
	UFUNCTION(BlueprintCallable)
		bool MakeDir(const FString& DirName);

	UFUNCTION(BlueprintCallable)
		bool RemoveDir(const FString& DirName);

	UFUNCTION(BlueprintCallable)
		void Download(const FString& localFilePath, const FString& RemoteFilePath);

	UFUNCTION(BlueprintCallable)
		void WindowUpload();
	UFUNCTION(BlueprintCallable)
		void Upload(const FString& filePath, const FString& RemoteFilePath);

	UFUNCTION(BlueprintCallable)
		bool RemoveFile(const FString& RemoteFilePath);
protected:

	UPROPERTY(BlueprintAssignable)
		FFTPDownloadDelegate DownloadCB;
	UPROPERTY(BlueprintAssignable)
		FFTPUploadDelegate UploadCB;
private:
		ftplib ftp;
		int DownState = -1;
		int UpState = -1;
		FString localPath;
		FString RemotePath;
};
