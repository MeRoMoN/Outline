// Fill out your copyright notice in the Description page of Project Settings.


#include "Outline/Public/Game/OutlineGameInstanceBase.h"
#include "JsonObjectConverter.h"


UOutlineGameInstanceBase::UOutlineGameInstanceBase()
{
	Http = &FHttpModule::Get();
}

void UOutlineGameInstanceBase::GoToMap(FString MapURL)
{
	GetWorld()->ServerTravel(MapURL);
}

void UOutlineGameInstanceBase::RetrieveServerList()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UOutlineGameInstanceBase::OnServerListProcessRequestComplete);

	Request->SetURL("https://localhost:44349/api/Host");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->ProcessRequest();
}

TArray<FServerData>& UOutlineGameInstanceBase::GetServerList()
{
	return ServerList;
}

void UOutlineGameInstanceBase::OnServerListProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	ServerList.Empty();
	if (Success)
	{
		FString ResponseStr = Response->GetContentAsString();
		ResponseStr.InsertAt(0, FString("{\"Response\":"));
		ResponseStr.AppendChar('}');
		UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *ResponseStr);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseStr);

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			TArray<TSharedPtr<FJsonValue>> JsonValues = JsonObject->GetArrayField(TEXT("Response"));

			for (TSharedPtr<FJsonValue> JsonValue : JsonValues)
			{
				FServerData ServerData = FServerData();
				TSharedPtr<FJsonObject> JsonObj = JsonValue->AsObject();

				if (FJsonObjectConverter::JsonObjectToUStruct(JsonObj.ToSharedRef(), &ServerData))
				{
					ServerList.Add(ServerData);
				}
			}

			OnServersListReceived.Broadcast();
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetServerList Failed"));
	}
}

