#pragma once

#include "stdhdr.h"







arStatus_t		phash_image(CByteImage_t &src, std::string &hash, const size_t SAMPLE_SIZE = 8);
ar_bool_t		phash_image_file(const wchar_t *file, std::string &hash);
size_t			phash_hamming_distance(const std::string &l, const std::string &r);

