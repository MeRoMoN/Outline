#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OutlineGameInstanceBase.generated.h"

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapURL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTexture2D* MapImage;
};


UCLASS()
class OUTLINE_API UOutlineGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:

	UOutlineGameInstanceBase();

protected:

	UFUNCTION(BlueprintCallable)
	void GoToMap(FString MapURL);
};