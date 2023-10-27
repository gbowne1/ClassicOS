#include "Version.h"

Version::Version()
		: major(0), minor(0), patch(0)
{
}

Version::Version(const unsigned int major, const unsigned int minor, const unsigned int patch)
		: major(major), minor(minor), patch(patch)
{
}

Version::Version(const Version& version)
		: major(version.major), minor(version.minor), patch(version.patch)
{
}

Version& Version::operator=(const Version& version)
{
	if (this == &version)
		return *this;

	major = version.major;
	minor = version.minor;
	patch = version.patch;

	return *this;
}

bool Version::operator==(const Version& version) const
{
	return major == version.major && minor == version.minor && patch == version.patch;
}

bool Version::operator!=(const Version& version) const
{
	return major != version.major || minor != version.minor || patch != version.patch;
}

unsigned int Version::operator[](const unsigned int i) const
{
	switch (i)
	{
		case 0:
			return major;
		case 1:
			return minor;
		case 2:
			return patch;
		default:
			return major;
	}
}

unsigned int& Version::operator[](const unsigned int i)
{
	switch (i)
	{
	case 0:
		return major;
	case 1:
		return minor;
	case 2:
		return patch;
	default:
		return major;
	}
}