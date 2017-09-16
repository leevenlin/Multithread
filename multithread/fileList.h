#pragma once
#ifndef _FILELIST_H_
#define _FILELIST_H_

#include <QFileDialog>
#include <set>
#include <string>
#include <io.h>
#include <sys/stat.h>
#include <QDebug>

class FileList
{
public:
	FileList();//不区分后缀名
	FileList(QString extName);
	FileList(QStringList extNameList);
	FileList(const FileList& list);
	bool hasNext() const;
	std::string getNextFileName();
	//FileList& operator = (const FileList& list);
	void resetIterator();

	//FileList& operator = (const FileList& list);

	bool addFiles();
	bool empty() const;

	QStringList getFileList() const;

private:
	void listFiles(QString fileExt);
	struct MyFileInfo
	{
		std::string fileName;
		long createdTime;
	};

	struct MyFileComp
	{
		bool operator()(const MyFileInfo &f1, const MyFileInfo &f2) const;
	};

	std::set<MyFileInfo, MyFileComp> files;
	std::set<MyFileInfo, MyFileComp>::iterator it;

	QString fileExtNames;
};

#endif