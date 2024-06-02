// Fill out your copyright notice in the Description page of Project Settings.


#include "DXTweakingDataObject.h"

UDXTweakingSelectNode::UDXTweakingSelectNode()
{
}

float UDXTweakingSelectNode::GetValueAsFloat()
{
	if (RawValue.IsEmpty())
	{
		return 0.0f;
	}
	if (RawValue.IsNumeric())
	{
		return FCString::Atof(*RawValue);
	}

	return 0.0f;
}

bool UDXTweakingSelectNode::GetValueAsBool()
{
	if (RawValue.IsEmpty())
	{
		// Return false if empty
		return false;
	}
	else if (RawValue.IsNumeric())
	{
		// Return true if numeric value is bigger than 0
		return (FCString::Atof(*RawValue) > 0.0f);
	}
	else
	{
		// Return true if value is "TRUE"
		return (RawValue.Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0);
	}
}

int32 UDXTweakingSelectNode::GetValueAsInteger()
{
	if (RawValue.IsEmpty())
	{
		return 0;
	}

	// Check value validity
	if (RawValue.IsNumeric())
	{
		int32 Index = -1;

		if (RawValue.FindChar(TEXT('.'), Index))
		{
			return FMath::TruncToInt(FCString::Atof(*RawValue));
		}
		else
		{
			return FCString::Atoi(*RawValue);
		}
	}
	
	return 0;
}

FString UDXTweakingSelectNode::GetValueAsString()
{
	return RawValue;
}

FName UDXTweakingSelectNode::GetValueAsName()
{
	return FName(*RawValue);
}

//==========TweakingDataUObject========================//

UDXTweakingDataObject::UDXTweakingDataObject()
{
	DataNodes.Empty();
}

int32 UDXTweakingDataObject::GetNumberOfRows()
{
	const int32 SingleRowNum = DataHeader.Num(); // Number of nodes in one row

	// Check if row number is valid
	if (SingleRowNum <= 0)
	{
		return -1;
	}

	// Check if data nodes count is perfectly divisible by the row count
	if (DataNodes.Num() % SingleRowNum != 0)
	{
		return -1;
	}

	return (int32)(DataNodes.Num() / SingleRowNum);
}

bool UDXTweakingDataObject::GetRow(int32 RowNumber, TArray<UDXTweakingSelectNode*>& OutNodes)
{
	// Check if the requested row number is valid
	if (RowNumber < 0 || RowNumber > (GetNumberOfRows() - 1))
	{
		return false;
	}

	// Add the requested data nodes into the node array
	for (int32 i = 0; i < DataHeader.Num(); i++)
	{
		// Get the node based on the row specified
		const int32 RowStart = RowNumber * DataHeader.Num();
		UDXTweakingSelectNode* NodeToAdd = DataNodes[i + RowStart];

		// Validity check, just in case
		if(NodeToAdd)
		{
			OutNodes.Add(NodeToAdd);
		}
	}
	return true;
}

bool UDXTweakingDataObject::ParseCsv(const FString& Filepath)
{
	
	DataHeader.Empty();
	DataNodes.Empty();

	// Update file origin for logging
	 FileOrigin = Filepath;
	
	const FString CsvString = *Filepath;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, CsvString);

	// Read through the array of strings from the CSV, checking for special characters to separate nodes
	int32 Iterator = 0;

	bool bIsHeader = true; // Used to check the first row of data as header
	bool bIsInQuote = false; // Used to mark if data cell is wrapped by a quotation mark, which means the entry contains a comma

	FString CurrentStr = TEXT("");

	while (CsvString.IsValidIndex(Iterator))
	{
		FString CurrentChar = CsvString.Mid(Iterator, 1);
		Iterator++;

		// Look for header entries
		if (bIsHeader)
		{
			// Checks for line break, end the header check
			if (CurrentChar.Equals(TEXT("\n")))
			{
				bIsHeader = false;

				AddToHeader(&CurrentStr);
				continue;
			}
			
			// Check for comma, add as new header entry
			if (CurrentChar.Equals(TEXT(",")))
			{
				AddToHeader(&CurrentStr);
				continue;
			}
			
			CurrentStr.Append(CurrentChar);
			continue;
		}

		// If in quote, look for end quote
		if (bIsInQuote)
		{
			if (CurrentChar.Equals(TEXT("\"")))
			{
				bIsInQuote = false;
				continue;
			}

			CurrentStr.Append(CurrentChar);
			continue;
		}

		// Check for starting quote
		if (CurrentChar.Equals(TEXT("\"")))
		{
			bIsInQuote = true;
			continue;
		}

		// Check for line break or comma, add as node entry
		if (CurrentChar.Equals(TEXT("\n")) || CurrentChar.Equals(TEXT(",")))
		{
			AddToNode(&CurrentStr);
			continue;
		}

		// Else append character
		CurrentStr.Append(CurrentChar);
	}

	return true;
}

void UDXTweakingDataObject::AddToNode(FString* StringToAdd)
{
	UDXTweakingSelectNode* NewNode = NewObject<UDXTweakingSelectNode>();
	if (NewNode)
	{
		NewNode->RawValue = *StringToAdd;
		DataNodes.Add(NewNode);
	}

	*StringToAdd = TEXT("");
}

void UDXTweakingDataObject::AddToHeader(FString* StringToAdd)
{
	DataHeader.Add(*StringToAdd);
	*StringToAdd = TEXT("");
}
