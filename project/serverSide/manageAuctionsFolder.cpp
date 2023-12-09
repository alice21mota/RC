#include "manageAuctionsFolder.h"

bool createAuctionsFolder() {
    filesystem::path directoryPath("AUCTIONS");
    return createFolder(directoryPath);
}

bool createAuctionFolder(string auctionsId) {
    filesystem::path directoryPath("AUCTIONS" + auctionsId);
    return createFolder(directoryPath);
}

bool createAuctionAssetFolder(string auctionsId) {
    filesystem::path directoryPath("AUCTIONS" + auctionsId + "/ASSET/");
    return createFolder(directoryPath);
}
