#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>
#include <cassert>

namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};
}

void MapChipField::ResetMapChipDate()
{ 
    mapChipDate_.date.clear(); 
    mapChipDate_.date.resize(kNumBlockVirtical);

    for (std::vector<MapChipType>& mapChipDateLine : mapChipDate_.date)
    {
		mapChipDateLine.resize(kNumBlockHorizontal);
    }
}

void MapChipField::LoadMapChipCsv(const std::string& filePath)
{ 
//マップチップデータをリセット
    ResetMapChipDate();

    //ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

    //マップチップCSV
	std::stringstream mapChipCsv;
    //ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
    //ファイルを閉じる
	file.close();

    //CSVからマップチップデータを読み込む
    for (uint32_t i = 0; i < kNumBlockVirtical; ++i)
    {
		std::string line;
		getline(mapChipCsv, line);

        //1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

        for (uint32_t j = 0; j < kNumBlockHorizontal; ++j)
        {
			std::string word;
			getline(line_stream, word, ',');

            if (mapChipTable.contains(word))
            {
				mapChipDate_.date[i][j] = mapChipTable[word];
            }
        }

    }


    }