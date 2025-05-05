// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_POSITIONAL_AUDIO_WIN32_INTERNALS_H_
#define MUMBLE_MUMBLE_POSITIONAL_AUDIO_WIN32_INTERNALS_H_

// These structures represent the header(s) of an NT image.
//
// There are two ways to read the image:
// - Loading the executable/library in memory and then reading the content at
//   the beginning.
// - Reading the process' memory at the beginning (base address).
//   In the case of a loaded library, its header(s) can be found at its load
//   (base) address.
//
// By reading the header(s) we can gather useful info regarding the process,
// such as the architecture and, in the case of a library, the address of an
// exported symbol.
struct ImageDosHeader {
	uint8_t magic[2]; // M Z
	uint16_t bytesOnLastPage;
	uint16_t pages;
	uint16_t relocations;
	uint16_t headerSizeInParagraphs;
	uint16_t minimumExtraParagraphs;
	uint16_t maximumExtraParagraphs;
	uint16_t initialSsValue;
	uint16_t initialSpValue;
	uint16_t checksum;
	uint16_t initialIpValue;
	uint16_t initialCsValue;
	uint16_t addressOfRelocationTable;
	uint16_t overlayNumber;
	uint16_t reserved[4];
	uint16_t oemIdentifier;
	uint16_t oemInfo;
	uint16_t reserved2[10];
	int32_t addressOfNtHeader;
};

struct ImageFileHeader {
	uint16_t machine;
	uint16_t numberOfSections;
	uint32_t timeDateStamp;
	uint32_t pointerToSymbolTable;
	uint32_t numberOfSymbols;
	uint16_t sizeOfOptionalHeader;
	uint16_t characteristics;
};

struct ImageNtHeadersNoOptional {
	uint8_t signature[4]; // P E \0 \0
	ImageFileHeader fileHeader;
};

struct ImageDataDirectory {
	uint32_t virtualAddress;
	uint32_t size;
};

struct ImageExportDirectory {
	uint32_t characteristics;
	uint32_t timeDateStamp;
	uint16_t majorVersion;
	uint16_t minorVersion;
	uint32_t name;
	uint32_t base;
	uint32_t numberOfFunctions;
	uint32_t numberOfNames;
	uint32_t addressOfFunctions;
	uint32_t addressOfNames;
	uint32_t addressOfNameOrdinals;
};

struct ImageOptionalHeader32 {
	uint16_t magic; // 0x10b
	uint8_t majorLinkerVersion;
	uint8_t minorLinkerVersion;
	uint32_t sizeOfCode;
	uint32_t sizeOfInitializedData;
	uint32_t sizeOfUninitializedData;
	uint32_t addressOfEntryPoint;
	uint32_t baseOfCode;
	uint32_t baseOfData;
	uint32_t imageBase;
	uint32_t sectionAlignment;
	uint32_t fileAlignment;
	uint16_t majorOperatingSystemVersion;
	uint16_t minorOperatingSystemVersion;
	uint16_t majorImageVersion;
	uint16_t minorImageVersion;
	uint16_t majorSubsystemVersion;
	uint16_t minorSubsystemVersion;
	uint32_t win32VersionValue;
	uint32_t sizeOfImage;
	uint32_t sizeOfHeaders;
	uint32_t checksum;
	uint16_t subsystem;
	uint16_t dllCharacteristics;
	uint32_t sizeOfStackReserve;
	uint32_t sizeOfStackCommit;
	uint32_t sizeOfHeapReserve;
	uint32_t sizeOfHeapCommit;
	uint32_t loaderFlags;
	uint32_t numberOfRvaAndSizes;
	ImageDataDirectory dataDirectory[16];
};

struct ImageOptionalHeader64 {
	uint16_t magic; // 0x20b
	uint8_t majorLinkerVersion;
	uint8_t minorLinkerVersion;
	uint32_t sizeOfCode;
	uint32_t sizeOfInitializedData;
	uint32_t sizeOfUninitializedData;
	uint32_t addressOfEntryPoint;
	uint32_t baseOfCode;
	uint64_t imageBase;
	uint32_t sectionAlignment;
	uint32_t fileAlignment;
	uint16_t majorOperatingSystemVersion;
	uint16_t minorOperatingSystemVersion;
	uint16_t majorImageVersion;
	uint16_t minorImageVersion;
	uint16_t majorSubsystemVersion;
	uint16_t minorSubsystemVersion;
	uint32_t win32VersionValue;
	uint32_t sizeOfImage;
	uint32_t sizeOfHeaders;
	uint32_t checksum;
	uint16_t subsystem;
	uint16_t dllCharacteristics;
	uint64_t sizeOfStackReserve;
	uint64_t sizeOfStackCommit;
	uint64_t sizeOfHeapReserve;
	uint64_t sizeOfHeapCommit;
	uint32_t loaderFlags;
	uint32_t numberOfRvaAndSizes;
	ImageDataDirectory dataDirectory[16];
};

struct ImageNtHeaders32 {
	uint8_t signature[4]; // P E \0 \0
	ImageFileHeader fileHeader;
	ImageOptionalHeader32 optionalHeader;
};

struct ImageNtHeaders64 {
	uint8_t signature[4]; // P E \0 \0
	ImageFileHeader fileHeader;
	ImageOptionalHeader64 optionalHeader;
};

#endif
