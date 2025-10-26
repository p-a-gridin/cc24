#include <stdio.h>
#include <getopt.h>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/mcc.hpp>

#define _CRT_SECURE_NO_WARNINGS 1

static std::string *in_file, *out_file, *c24_file;

static void usage();
static int process();

int main(int argc, char *argv[])
{
    for(;;) {
        int c = getopt(argc, argv, "i:o:c:");
        if(c == -1)
            break;

        switch(c) {
        case 'i':
            if(!optarg) {
                usage();
                return 1;
            }
            in_file = new std::string(optarg);
            break;
        case 'o':
            if(!optarg) {
                usage();
                return 1;
            }
            out_file = new std::string(optarg);
            break;
        case 'c':
            if(!optarg) {
                usage();
                return 1;
            }
            c24_file = new std::string(optarg);
            break;

        case '?':
        case 'h':
        default:
            usage();
            return 1;
            break;
        }
    }

    if(in_file == nullptr || out_file == nullptr || c24_file == nullptr) {
        usage();
        return 1;
    }

    return process();
}

void usage()
{
    printf("cc24 cc24 -i input_image -c color-checker-24_image -o output_image\n");
}

int process()
{
    cv::Mat imat = cv::imread(in_file->c_str(), cv::IMREAD_COLOR);
    if(!imat.data) {
        printf("Invalid input file \"%s\": %s\n", in_file->c_str(), strerror(errno));
        return 1;
    }

    cv::Mat cmat = cv::imread(c24_file->c_str(), cv::IMREAD_COLOR);
    if(!cmat.data) {
        printf("Invalid ColorChecker 24 file \"%s\": %s\n", in_file->c_str(), strerror(errno));
        return 1;
    }

    cv::cvtColor(cmat, cmat, cv::COLOR_RGB2BGR);

    cv::Ptr<cv::mcc::CCheckerDetector> detector = cv::mcc::CCheckerDetector::create();
    cv::mcc::TYPECHART chartType = cv::mcc::TYPECHART(0);
    if(!detector->process(cmat, chartType, 1)) {
        printf("ColorChecker 24 photo is bad\n");
        return 1;
    }

    return 0;
}
