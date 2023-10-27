#pragma once

/// A helper class for storing version major, minor and patch.
class Version
{
public:
	unsigned int major;
	unsigned int minor;
	unsigned int patch;

	/// Default members initialization.
	Version();

	/// Initializes members with given major, minor and patch.
	/// @param [in] major The major version.
	/// @param [in] minor The minor version.
	/// @param [in] patch The patch version.
	Version(const unsigned int major, const unsigned int minor, const unsigned int patch);

	/// Copies all members from the given version object.
	/// @param [in] version The version object to copy from.
	Version(const Version& version);

	/// Copies all members from the given version object.
	/// @param [in] version The version object to copy from.
	/// @returns The version object that has been assigned to.
	Version& operator=(const Version& version);

	bool operator==(const Version& version) const;

	bool operator!=(const Version& version) const;

	unsigned int operator[](const unsigned int i) const;

	unsigned int& operator[](const unsigned int i);
};