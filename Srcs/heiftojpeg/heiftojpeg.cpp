#include "hevcimagefilereader.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if ( argc != 3 ) {
        cout << "usage: heiftojpeg <input_file_name> <output_file_name>";
        return 1;
    }

    char *input_file_name = argv[1];
    char *output_file_name = argv[2];

    cout << "converting " << input_file_name << " to " << output_file_name;

    HevcImageFileReader reader;
    ImageFileReaderInterface::DataVector data;
    ImageFileReaderInterface::IdVector itemIds;

    reader.initialize(input_file_name);
    const auto& properties = reader.getFileProperties();

    // Verify that the file has one or several images in the MetaBox
    if (not (properties.fileFeature.hasFeature(ImageFileReaderInterface::FileFeature::HasSingleImage) ||
        properties.fileFeature.hasFeature(ImageFileReaderInterface::FileFeature::HasImageCollection)))
    {
        printf("Input has no image\n");
        return 1;
    }


    // Find the item ID of the first master image
    const uint32_t contextId = properties.rootLevelMetaBoxProperties.contextId;
    cout << "A"
    reader.getItemListByType(contextId, "master", itemIds);
    cout << "B"
    const uint32_t masterId = itemIds.at(0);
    cout << "C"

    const auto& metaBoxFeatures = properties.rootLevelMetaBoxProperties.metaBoxFeature; // For convenience
    if (metaBoxFeatures.hasFeature(ImageFileReaderInterface::MetaBoxFeature::HasThumbnails))
    {
        // Thumbnail references ('thmb') are from the thumbnail image to the master image
        reader.getReferencedToItemListByType(contextId, masterId, "thmb", itemIds);
        const uint32_t thumbnailId = itemIds.at(0);

        reader.getItemDataWithDecoderParameters(contextId, thumbnailId, data);
        // ...decode data and display the image, show master image later
    }
    else
    {
        // There was no thumbnail, show just the master image
        reader.getItemDataWithDecoderParameters(contextId, masterId, data);
        // ...decode and display...
    }

    return 0;
}
