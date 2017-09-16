#include "fileList.h"

FileList::FileList()
{
	//listFiles(QString("*.dat"));
	this->fileExtNames = "*.*";
}

FileList::FileList(QString extName)
{
	//listFiles(extName);
	this->fileExtNames = "*.dat";
}

FileList::FileList(QStringList extNameList)
{
	QString names;
	QStringListIterator qstrIt(extNameList);
	while (qstrIt.hasNext())
	{
		names += qstrIt.next() + " ";
	}
	//listFiles(names);
	this->fileExtNames = names;
}

bool FileList::hasNext() const
{
	return it != files.end();
}

std::string FileList::getNextFileName()
{
	return (*it++).fileName;
}

void FileList::resetIterator()
{
	it = files.begin();
}

bool FileList::addFiles()
{
	QStringList fileList;
	QString tmp = QString("Image Files (") + fileExtNames + QString(")");
	if ((fileList = QFileDialog::getOpenFileNames(0, QFileDialog::tr("choose files"), 
		".", QFileDialog::tr(tmp.toStdString().data()))).empty())
		return false;

	struct _stat buf;
	QListIterator<QString> qstrIt = QListIterator<QString>(fileList);
	while (qstrIt.hasNext())
	{
		MyFileInfo fileInfo;
		fileInfo.fileName = qstrIt.next().toStdString();
		_stat(fileInfo.fileName.data(), &buf);
		fileInfo.createdTime = buf.st_atime;
		this->files.insert(fileInfo);
	}

	this->it = files.begin();

	return true;
}

bool FileList::empty() const
{
	return files.empty();
}

QStringList FileList::getFileList() const
{
	QStringList strList;
	std::set<MyFileInfo, MyFileComp>::iterator itTmp;
	for (itTmp=files.begin(); itTmp!=files.end(); itTmp++)
	{
		strList.append(QString((*itTmp).fileName.data()));
	}

	return strList;
}

void FileList::listFiles(QString fileExt)
{
	QStringList fileList;
	QString tmp = QString("Image Files (") + fileExt + QString(")");
	while((fileList = QFileDialog::getOpenFileNames(0, QFileDialog::tr("choose files"), 
		".", QFileDialog::tr(tmp.toStdString().data()))).empty());
	struct _stat buf;
	QListIterator<QString> qstrIt = QListIterator<QString>(fileList);
	while (qstrIt.hasNext())
	{
		MyFileInfo fileInfo;
		fileInfo.fileName = qstrIt.next().toStdString();
		_stat(fileInfo.fileName.data(), &buf);
		fileInfo.createdTime = buf.st_atime;
		this->files.insert(fileInfo);
	}

	this->it = files.begin();
}

bool FileList::MyFileComp::operator()(const MyFileInfo &f1, const MyFileInfo &f2) const
{
	return (f1.createdTime < f2.createdTime);
}