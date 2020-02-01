
/**
 * Check to keep the expanded state of categories
 */
UPROPERTY(config, EditAnywhere, Category = "Graph Menu")
bool bKeepCategoriesExpanded = false;

// List of categories that have been expanded
UPROPERTY(config)
TArray<FString> expandedCategories;