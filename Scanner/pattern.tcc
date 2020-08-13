
template <typename TColor, int PatternWidth, int PatternHeight> Pattern<TColor, PatternWidth, PatternHeight>::Pattern(const Image& image): grade_(1)
{
    int cx, cy, mpix, mpiy, cpx1, cpy1, cpx2, cpy2;
    TColor q11, q12, q21, q22, r1, r2;
    TColor new_inverted_pixel;
    TColor summary_color;
    TColor summary_color_y;
    TColor summary_color_x;
    for (unsigned int h = 0; h < PatternHeight; ++h)
        for (unsigned int w = 0; w < PatternWidth; ++w)
        {
            static const int c = 4096;

            cx = ((w*2 + 1)*(c/2))/PatternHeight;
            cy = ((h*2 + 1)*(c/2))/PatternWidth;

            mpix = ((cx*image.getWidth())/(c/2) + 1)/2;
            mpiy = ((cy*image.getHeigth())/(c/2) + 1)/2;

            // какой-то баг компилятора, видимо. Без принудительного приведения типов при делении знакового на беззнаковое приводится к беззнаковому.
            cpx1 = ((mpix*2 - 1)*(c/2))/(signed int)(image.getWidth());
            cpy1 = ((mpiy*2 - 1)*(c/2))/(signed int)(image.getHeigth());

            cpx2 = ((mpix*2 + 1)*(c/2))/(signed int)(image.getWidth());
            cpy2 = ((mpiy*2 + 1)*(c/2))/(signed int)(image.getHeigth());

            if (mpiy)
            {
                q11 = mpix? image(mpiy - 1, mpix - 1): TColor().getInverted();
                q12 = ((unsigned int)mpix < image.getWidth())? image(mpiy - 1, mpix): TColor().getInverted();

                r1 = (q11*(cpx2 - cx) + q12*(cx - cpx1))/(cpx2 - cpx1);
            }
            else
                r1 = TColor().getInverted();
            if ((unsigned int)mpiy < image.getHeigth())
            {
                q21 = mpix? image(mpiy, mpix - 1): TColor().getInverted();
                q22 = ((unsigned int)mpix < image.getWidth())? image(mpiy, mpix): TColor().getInverted();

//                std::cout << mpix << " " << mpiy << std::endl;
//                std::cout << image(mpiy, mpix) << std::endl;
//                std::cout << q21 << " " << q22 << std::endl;
                r2 = (q21*(cpx2 - cx) + q22*(cx - cpx1))/(cpx2 - cpx1);
//                std::cout << cpx << " " << cdx << " " << cx << " " << ((mpix*2 - 1)*(c/2))/image.getWidth() << std::endl;
//                std::cout << mpix*2 - 1 << " " << ((mpix*2 - 1)*(c/2)) << " " << ((mpix*2 - 1)*(c/2))/image.getWidth() << std::endl;
//                std::cout << - 100/(signed int)(image.getWidth()) << " " << image.getWidth()<< std::endl;
            }
            else
            {
                r2 = TColor().getInverted();
            }
            new_inverted_pixel = ((r1*(cpy2 - cy) + r2*(cy - cpy1))/(cpy2 - cpy1)).getInverted();

            inverted_colors_matrix[h][w] = new_inverted_pixel;
            summary_color_y += new_inverted_pixel*h;
            summary_color_x += new_inverted_pixel*w;
            summary_color += new_inverted_pixel;
        }
    summary_intensity_ = summary_color.getIntensity();
    average_intensity_ = summary_intensity_/(PatternHeight*PatternWidth);
    intensity_center_y_ = summary_color_y.getIntensity()/summary_intensity_;
    intensity_center_x_ = summary_color_x.getIntensity()/summary_intensity_;
}

template<typename TColor, int PatternWidth, int PatternHeight>
void Pattern<TColor, PatternWidth, PatternHeight>::merge(const Pattern<TColor>& a)
{
    TColor summary_color;
    TColor summary_color_y;
    TColor summary_color_x;
    TColor new_value;

//    std::cout << "merge " << sizeof(inverted_colors_matrix) << " " << sizeof(TColor) << std::endl;
    inverted_colors_matrix[1][51] = TColor();
    inverted_colors_matrix[1][49] = TColor();
    inverted_colors_matrix[1][50] = TColor();
    inverted_colors_matrix[1][51] = TColor();
    for (unsigned int h = 0; h < PatternHeight; ++h)
        for (unsigned int w = 0; w < PatternWidth; ++w)
        {
//            std::cout << inverted_colors_matrix[h][w] << " " << a.inverted_colors_matrix[h][w] << " " << grade_ << " " <<  a.grade_ << std::endl;
//            std::cout << inverted_colors_matrix[h][w]*grade_ << " " << a.inverted_colors_matrix[h][w]*a.grade_ << std::endl;
            new_value = (inverted_colors_matrix[h][w]*grade_ + a.inverted_colors_matrix[h][w]*a.grade_) / (grade_ + a.grade_);
//            std::cout << h << " " << w << " " << new_value << std::endl;
//            std::cout << new_value << std::endl;
            inverted_colors_matrix[h][w] = new_value;
            summary_color_y += new_value*h;
            summary_color_x += new_value*w;
            summary_color += new_value;
//            if (new_value.getIntensity() > 256)
//                std::cout << "PROBLEM there" << std::endl;
        }
    summary_intensity_ = summary_color.getIntensity();
    average_intensity_ = summary_intensity_/(PatternHeight*PatternWidth);
    intensity_center_y_ = summary_color_y.getIntensity()/summary_intensity_;
    intensity_center_x_ = summary_color_x.getIntensity()/summary_intensity_;

    grade_ += a.grade_;
    if (grade_ > Max_Grade_)
        grade_ = grade_;
}

template<typename TColor, int PatternWidth, int PatternHeight>
int Pattern<TColor, PatternWidth, PatternHeight>::
corellation(const Pattern<TColor, PatternWidth, PatternHeight>& a, const Pattern<TColor, PatternWidth, PatternHeight>& b, const CorellationType type)
{
//        std::cout << " " << a.intensity_center_y_ << " " << b.intensity_center_y_ << " " << a.intensity_center_x_ << " " <<  b.intensity_center_x_ << std::endl;
        const int int_cent_dy = a.intensity_center_y_ - b.intensity_center_y_;
        const int int_cent_dx = a.intensity_center_x_ - b.intensity_center_x_;
        const int ady = std::max(0, -int_cent_dy);
        const int bdy = std::min(0, -int_cent_dy);
        const int adx = std::max(0, -int_cent_dx);
        const int bdx = std::min(0, -int_cent_dx);
        const int ratio = (is_need_opt_(type, Normalization))? (64*a.summary_intensity_)/b.summary_intensity_: 64;
        const int average_a = (is_need_opt_(type, Balancing))? a.average_intensity_: 0;
        const int average_b = (is_need_opt_(type, Balancing))? b.average_intensity_: 0;

//        std::cout << " " << int_cent_dx << " " << int_cent_dy << " " << ratio << " " << average_a << " " << average_b << std::endl;
        int corellation = 0;
        for (int h = abs(int_cent_dy); h < PatternHeight; ++h)
        {
            for (int w = abs(int_cent_dx); w < PatternWidth; ++w)
            {
                if ((h - ady) < 0 || (h - ady) > 255 || (w - adx) < 0 || (w - adx) > 255 || (h + bdy) < 0 || (h + bdy) > 255 || (w + bdx) < 0 || (w + bdx) > 255)
                    std::cout << "PROBLEM" << std::endl;
                corellation += TColor::color_corellation[type]( a.inverted_colors_matrix[h - ady][w - adx] - average_a, (b.inverted_colors_matrix[h + bdy][w + bdx]*ratio)/64 - average_b);
            }
        }

        return corellation;
}

template<typename TColor, int PatternWidth, int PatternHeight>
void Pattern<TColor, PatternWidth, PatternHeight>::dump__(const std::string& path, const std::string& prefix) const
{
    static unsigned int num = 0;
    const unsigned char headers[0x36] =
    {
        // BITMAPFILEHEADER
        //     WORD  bfType;
        0x42, 0x4d,
        //     DWORD bfSize;
        0x36, 0x00, 0x03, 0x00,
        //     WORD  bfReserved1;
        0x00, 0x00,
        //     WORD  bfReserved2;
        0x00, 0x00,
        //     DWORD bfOffBits;
        0x36, 0x00, 0x00, 0x00,
        // BITMAPINFOHEADER
        //     DWORD biSize;
        0x28, 0x00, 0x00, 0x00,
        //     LONG  biWidth;
        0x00, 0x01, 0x00, 0x00,
        //     LONG  biHeight;
        0x00, 0x01, 0x00, 0x00,
        //     WORD  biPlanes;
        0x01, 0x00,
        //     WORD  biBitCount;
        0x18, 0x00,
        //     DWORD biCompression;
        0x00, 0x00, 0x00, 0x00,
        //     DWORD biSizeImage;
        0x00, 0x00, 0x03, 0x00,
        //     LONG  biXPelsPerMeter;
        0x26, 0x0e, 0x00, 0x00,
        //     LONG  biYPelsPerMeter;
        0xff, 0x0d, 0x00, 0x00,
        //     DWORD biClrUsed;
        0x00, 0x00, 0x00, 0x00,
        //     DWORD biClrImportant;
        0x00, 0x00, 0x00, 0x00,
    };

    unsigned char bitmap[0x30000];

    unsigned char* b = bitmap;

    for (int h = PatternHeight - 1; h >= 0; --h)
        for (unsigned int w = 0; w < PatternWidth; ++w)
        {
            *b++ = inverted_colors_matrix[h][w].getInverted().getBlue();
            *b++ = inverted_colors_matrix[h][w].getInverted().getGreen();
            *b++ = inverted_colors_matrix[h][w].getInverted().getRed();
        }


    string filename = path + "/" + prefix + std::to_string(num) + ".bmp";

    std::ofstream file(filename, std::ios::binary);
    if (file) {
		file.write((const char*const)headers, 0x36);
		file.write((const char*const)bitmap, 0x30036);
		file.close();
    } else {
		std::cout << "No such file " << filename.str() << std::endl;
    }
    num++;
}
