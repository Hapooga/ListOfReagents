#include "ExcelFile.h"


ExcelFile::ExcelFile(const QString& excelLoadPath,  QObject *parent)
    : QObject(parent)
{ 
#pragma region Настройки заголовка столбца {
    header.setPatternBackgroundColor(QColor(250,238,199));
    header.setBorderStyle(QXlsx::Format::BorderThin);
    header.setFontColor(QColor(Qt::black));
    header.setFontBold(true);
    header.setFontSize(14);
#pragma endregion }

#pragma region Чтение xml {

    if(not excelLoadPath.isEmpty()) {
        QXlsx::Document xlsxFile(excelLoadPath);

        if (xlsxFile.load()) {

            excelFileData.reserve(xlsxFile.sheetNames().size());

            ushort FIRST_CELL = 1;
            for(const auto& list : xlsxFile.sheetNames()) {

                xlsxFile.selectSheet(list);

                std::vector<std::vector<QString>> sheetBuffer;
                for(ushort iC = FIRST_CELL; iC <= SheetSize.HEIGHT; ++iC) {

                     std::vector<QString> rowBuffer;
                 rowBuffer.reserve(SheetSize.WIDTH);
                    for (ushort iR = FIRST_CELL; iR <= SheetSize.WIDTH; ++iR) {

                         Cell* cell = xlsxFile.cellAt(iC, iR);
                        if (cell) {
                              rowBuffer.push_back(cell->readValue().toString()); //QVariant
                        } else {
                             rowBuffer.push_back(QString());
                        }

                    }
                    sheetBuffer.push_back(std::move(rowBuffer));
                }
                this->excelFileData.push_back(std::make_pair(std::move(list), std::move(sheetBuffer)));
            }
         }
    }
#pragma endregion }
}


const void ExcelFile::CreateExcelDocument(const std::vector<std::pair<SheetName,Sheet>>& sheets, const QString& fullNamePath) const {

    QXlsx::Document xlsxFile(fullNamePath);

    const ushort FIRSTROW = 1, FIRSTCOL = 1;

    for(const auto& [sheetName, sheet] : sheets) { // идем по листам

        xlsxFile.addSheet(sheetName); // добавляем лист
        xlsxFile.autosizeColumnWidth(); // авторазмер ячеек

        ushort rowCounter = FIRSTROW;
        for(const auto& row : sheet) { //идем по строкам

            ushort colCounter = FIRSTCOL;
            for(const auto& cellData : row) { // по клеткам строки
                if(rowCounter == FIRSTROW) // если заголовок
                xlsxFile.write(rowCounter, colCounter, cellData, this->header); // записываем в ячейку с настройками заголовка
                ++colCounter;
            }
            ++rowCounter;
        }
    }

    xlsxFile.save();
}



const QStringList ExcelFile::GetSheetsNames() const { return {excelFileData.begin()->first, excelFileData.end()->first} ;}
const std::vector<std::pair<SheetName,Sheet>>& ExcelFile::GetSheets() const { return excelFileData; }



