from generator.loaders import ProfileLoader, read_profile


def test_read_profiles_301():
    """Test reading and validating profiles."""
    profile = read_profile(301)
    assert isinstance(profile, dict)
    assert len(profile) > 0

    assert "profiles" in profile
    assert 301 in profile["profiles"]
    profile = profile["profiles"][301]

    assert "name" in profile
    assert "description" in profile
    assert isinstance(profile["name"], str)
    assert isinstance(profile["description"], str)


def test_load_profiles():
    """Test loading all profiles."""
    p = ProfileLoader()
    p.load_all()
