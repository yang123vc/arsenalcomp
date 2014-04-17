#pragma once

#include "stdhdr.h"



typedef CImg<ar_byte_t> CByteImage_t;


arStatus_t		phash_image(CByteImage_t &src, std::string &hash, const size_t SAMPLE_SIZE = 8);
ar_bool_t		phash_image_file(const wchar_t *file, std::string &hash);



