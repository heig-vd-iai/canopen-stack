import warnings

with warnings.catch_warnings(record=True) as w:
    warnings.simplefilter("always")

    try:
        data = readConfig("config2.yaml")
        config = SchemaConfig.model_validate(data)

        data = readProfile(301)
        profile301 = SchemaProfile.model_validate(data)

        data = readProfile(402)
        profile402 = SchemaProfile.model_validate(data)

        profiles = merge_profiles(profile301, profile402)

    except FileNotFoundError as e:
        print(f"Configuration file not found: {e}")
        config = None
    except ValidationError as e:
        print("Validation error(s):")
        print(format_validation_errors(e, data))
        if STRICT_VALIDATION:
            raise
        else:
            config = None  # ou retour anticipé, ou status code
    else:
        if w:
            print("\n=== WARNINGS ===")
            for warning in w:
                print(f"- {warning.message}")
            print()
