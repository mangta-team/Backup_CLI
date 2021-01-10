#include "Command.hpp"

using namespace std;	
using namespace backup::core;

backup::Abstract_Command::Abstract_Command(bfs::path running_path)
{
	this->running_path = running_path;
	this->path_manager = new Path_Manager(this->running_path);

	// Checker의 영역이 아닌 에러이므로 Command에서 처리함
	if (!bfs::exists(this->path_manager->get_management_file()))
	{
		cout << "[알림] >> 관리 파일을 찾을 수 없습니다. 재생성합니다." << endl;
		this->path_manager->create_management_file();
	}
}

void backup::Add_Command::excute()
{
	this->path_manager->push_back(this->root, this->destination);
	this->path_manager->list_to_file();
}

void backup::Delete_Command::excute()
{	
	// Checker의 영역이 아닌 에러이므로 Command에서 처리함
	if (this->path_manager->management_file_empty())
	{
		cout << "[알림] >> 파일이 비어있습니다." << endl;
		return;
	}

	if (this->index == -1)
		this->path_manager->clear();
	else
		this->path_manager->erase(this->index);
}

void backup::Print_Command::excute()
{
	this->path_manager->print();
}

void backup::Sync_Command::excute()
{
	// Checker의 영역이 아닌 에러이므로 Command에서 처리함
	if (this->path_manager->management_file_empty())
	{
		cout << "[알림] >> sync list is empty." << endl;
		return;
	}

	try
	{
		cout << "[알림] >> 다음 항목을 동기화 합니다." << endl << endl;
			
		this->path_manager->print();

		for (auto path_management_data : this->path_manager->get_path_list())
		{
			backup::core::Sync sync;
			sync.execute(this->path_manager->get_root(path_management_data), 
				this->path_manager->get_destination(path_management_data));
		}		
	
		cout << "[알림] >> 동기화가 끝났습니다." << endl << endl;
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
}

void backup::Help_Command::excute()
{
}

void backup::Exit_Command::excute()
{
}

