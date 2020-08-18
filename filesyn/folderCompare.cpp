#include "folderCompare.h"
#include <boost/filesystem.hpp>

folderCompare::folderCompare( )
{
}

folderCompare::folderCompare(boost::filesystem::path path1, boost::filesystem::path path2)
{
	compare = std::make_pair(path1, path2);
}

folderCompare::~folderCompare( )
{
}

void folderCompare::setFolderCompare(boost::filesystem::path &path1, boost::filesystem::path &path2)
{
	compare = std::make_pair(path1, path2);
}

boost::filesystem::path folderCompare::getCompareFirst( )
{
	return compare.first;
}

boost::filesystem::path folderCompare::getCompareSecond( )
{
	return compare.second;
}

std::pair<boost::filesystem::path, boost::filesystem::path> folderCompare::getFolderCompare( )
{
	return compare;
}


folderCompareSys::folderCompareSys( )
{
}

folderCompareSys::~folderCompareSys( )
{
}

void folderCompareSys::scan( )
{
	scanPath(compare.first);
	scanPath(compare.second);
}

void folderCompareSys::scanPath(boost::filesystem::path& path)
{
	if (!boost::filesystem::exists(path))
	{
		return;
	}
	//globalSetting* test = &globalSetting::GetSetting( );
	//globalSetting::GetSetting( ).setRoot(root);
	if (boost::filesystem::is_directory(path)) //ȷ����Ŀ¼
	{
		boost::filesystem::recursive_directory_iterator end_iter;
		//������������ʼ����
		for (boost::filesystem::recursive_directory_iterator iter(path); iter != end_iter; iter++)
		{
			try
			{//������, �������ļ���Ϣ��
				if (boost::filesystem::is_regular_file(iter->path( )))
				{
					boost::shared_ptr<fileInfo> info(new fileInfo(iter->path( )));
					fileInfoPtr.push_back(info);
				}
			}
			catch (const std::exception& ex)
			{
				std::cout << "����: " << ex.what( ) << std::endl;
				continue;
			}
		}

	}
}

void folderCompareFtp::scan( )
{

}

folderCompareFtp::folderCompareFtp( )
{
}

folderCompareFtp::~folderCompareFtp( )
{
}

