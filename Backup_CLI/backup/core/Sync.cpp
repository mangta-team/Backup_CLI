#include "Sync.hpp"

using namespace std;

// TODO 함수를 더 작게 만들기
// 1. 경로 "dest/root.filename()"이 없으면 root.filename() 디렉토리 생성
// 2. source의 하위 폴더, 파일을 vector에 저장
// 3. 하위 폴더가 있으면 sync(하위폴더, dest/source.filename()) 호출 (하위 폴더 반복)
// 4. 하위 폴더가 더이상 없으면 파일들 복사
void backup::core::sync(const bfs::path& root, const bfs::path& destination)
{
	// 1. 경로 "dest/root.filename()"이 없으면 root.filename() 디렉토리 생성
	// 예) root: C:/A, dest: D:/Backup -> D:/Backup/A
	bfs::path dest = destination.generic_path();
	if (!bfs::exists(dest /= root.filename()))
		bfs::create_directory(dest.generic_path());

	// 2. source의 하위 폴더, 파일을 vector에 저장
	vector<bfs::path> directoriy_group;
	vector<bfs::path> file_group;
	
	for(auto copy_path : bfs::directory_iterator(root))
	{
		if (bfs::is_directory(copy_path.path().generic_path()))
			directoriy_group.push_back(copy_path.path().generic_path());

		else
			file_group.push_back(copy_path.path().generic_path());
	}

	// 3. 하위 폴더가 있다면 재귀함수로 sync 호출
	if (!directoriy_group.empty())
	{
		for (int index = 0; index < directoriy_group.size(); index++)
		{
			sync(directoriy_group[index], dest);
		}
	}
	
	// 4. 파일 복사
	if (!file_group.empty())
	{
		for (int index = 0; index < file_group.size(); index++)
		{
			dest = destination;
			dest /= root.filename() /= file_group[index].filename();
			bfs::copy_file(file_group[index], dest, bfs::copy_option::overwrite_if_exists);
		}
	}
}

void backup::core::Sync::init(bfs::path root)
{
	for (auto copy_path : bfs::directory_iterator(root))
	{
		if (bfs::is_directory(copy_path.path().generic_path()))
			this->directoriy_group.push_back(copy_path.path().generic_path());

		else
			this->file_group.push_back(copy_path.path().generic_path());
	}
}

void backup::core::Sync::copy_directory_group(bfs::path destination)
{
	if (!this->directoriy_group.empty())
		for (auto directory : directoriy_group)
			int i;
			//execute(directory, destination);
}

void backup::core::Sync::copy_file_group(bfs::path& destination)
{

	if (!this->file_group.empty())
		for (auto file : this->file_group)
		{
			bfs::path dest = destination.append(file.filename()).generic_path();
			bfs::copy_file(file, dest, bfs::copy_option::overwrite_if_exists);
		}
}

void backup::core::Sync::sync(bfs::path& destination)
{
	// 폴더 복사 후 파일 복사, 파일 복사시 폴더가 존재하지 않으면 에러 발생
	copy_directory_group(destination);

	copy_file_group(destination);
}

void backup::core::Sync::execute(bfs::path& root, bfs::path& destination)
{
	// 경로 "dest/root.filename()"이 없으면 root.filename() 디렉토리 생성
	// 예) root: C:/Root/A, dest: D:/Backup -> D:/Backup/A
	bfs::path dest = destination.append(root.filename()).generic_path();
	if (!bfs::exists(dest))
		bfs::create_directory(dest);

	// 복사해야 할 목록 초기화
	init(root);

	sync(destination);
}
