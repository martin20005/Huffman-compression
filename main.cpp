#define HUFFMAN_TEST

#include <iostream>
#define MEMTRACE
#include "gtest_lite.h"
#include "memtrace.h"
using namespace gtest_lite;
#include "bitbuffer.h"
#include "letter.h"
#include "huffman.h"

#ifdef HUFFMAN_TEST
int main() {
    // ------------------------------  Buffer  ------------------------------ //
    TEST(Buffer, Constructor) {
        BitBuffer bb0(8);
        bb0.push('a', 4);
        bb0.close();
        BitBuffer bb1 = BitBuffer();
        EXPECT_EQ(sizeof(char)*8, (unsigned long) bb1.sizeOfChunk());
        BitBuffer bb2(bb0);
        EXPECT_EQ(bb0.pop(), bb2.pop());
    } END

    TEST(Buffer, Push) {
        BitBuffer test1(sizeof(char));
        test1.push('a' >> 4, 4);
        test1.push('a', 4);
        test1.close();
        EXPECT_EQ('a', test1.data()[0]);
    } END

    TEST(Buffer, Pop) {
        BitBuffer test2 = BitBuffer();
        test2.push('a', 8);
        test2.close();
        EXPECT_EQ(1, test2.countOfFullChunks());
        EXPECT_EQ('a', test2.pop());
        EXPECT_EQ(0, test2.countOfFullChunks());
    } END

    TEST(Buffer, Bit) {
        BitBuffer test3 = BitBuffer();
        test3.push(170, 8);
        test3.close();
        int cnt = 0;
        for (int i = 0; i < test3.sizeOfChunk(); ++i) {
            cnt += test3.bit() ? 1 : 0;
        }
        EXPECT_EQ(4, cnt);
    } END

    // ------------------------------  Letter  ------------------------------ //
    TEST(Letter, Constructor) {
        Letter l0 = Letter();
        EXPECT_EQ((char) 0, l0.original());
        EXPECT_EQ((long long int) 0, l0.huffman());
        EXPECT_EQ((unsigned char) 0, l0.length());
        Letter l1 = Letter('a');
        EXPECT_EQ('a', l1.original());
    } END

    TEST(Letter, Functions) {
        Letter l0('a');
        l0.huffman('b', 4);
        Letter l1 = l0;
        EXPECT_EQ(l0.original(), l1.original());
        EXPECT_EQ(l0.huffman(), l1.huffman());
        EXPECT_TRUE(l0 == l1);
    } END

    // ------------------------------  Huffman  ------------------------------ //

    string HOME = std::getenv("HOME") ? std::getenv("HOME") : ".";
    string source = HOME + "/Prog/C++/Prog2/hf/huffman_1.2/Test.txt";
    string destiny = HOME + "/Prog/C++/Prog2/hf/huffman_1.2/Test.huffman_code_";
    string extdest = HOME + "/Prog/C++/Prog2/hf/huffman_1.2/Test_ext.txt";
    std::ifstream source_file;
    std::ofstream destiny_file;

    Huffman h2 = Huffman();

    source_file.open(source);
    destiny_file.open(destiny);
    h2.compress(source_file, destiny_file);
    source_file.close();
    destiny_file.close();

    source_file.open(destiny);
    destiny_file.open(extdest);
    h2.extract(source_file, destiny_file);
    source_file.close();
    destiny_file.close();

    return 0;
}
#else
int main() {
    /* The desired behaviour of the Huffman compression program ((Homework)) */
    string input_line;
    getline(cin, input_line);
    //examples:
    //compress "input/file/path" "output/file/path"
    //extract "input/file/path" "output/file/path"

    int command_id;
    command_id = input_line.rfind("compress ", 0) == 0 ? 1 : -1;
    command_id = input_line.rfind("extract ", 0) == 0 ? 2 : command_id;

    int path_start = input_line.find('\"', 7) + 1; //7, so command skipped
    int len = input_line.find('\"', path_start) - path_start;
    string input_path = input_line.substr(path_start, len);

    string output_path;
    path_start += len + 2;
    path_start = input_line.find('\"', path_start) + 1;
    if (path_start <= 0) {
        output_path = input_path + ((command_id == 1) ? ".huff" : "_ext.txt");
    } else {
        len = input_line.find('\"', path_start) - path_start;
        output_path = input_line.substr(path_start, len);
    }
    switch (command_id) {
        case 1: {
            ifstream input_file;
            input_file.open(input_path);
            if (!input_file.is_open())
                { std::cout << "No such file" << std::endl; break; }
            ofstream output_file;
            output_file.open(output_path);
            Huffman compressor = Huffman();
            compressor.compress(input_file, output_file);
            input_file.close();
            output_file.close();
            break;
        }
        case 2: {
            ifstream input_file;
            input_file.open(input_path);
            if (!input_file.is_open())
                { std::cout << "No such file" << std::endl; break; }
            ofstream output_file;
            output_file.open(output_path);
            Huffman extractor = Huffman();
            extractor.extract(input_file, output_file);
            input_file.close();
            output_file.close();
            break;
        }
        default: {
            std::cout << "Invalid command" << std::endl;
            break;
        }
    }
    return 0;
}
#endif
