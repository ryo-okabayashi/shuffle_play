#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>

using namespace std;

char * play_random_flac(const char * player, const char * dirname) {
	DIR *dp;
	struct stat st;
	struct dirent *dir;
	dp = opendir(dirname);
	vector<string> dirs;
	vector<string> flacs;

	// ディレクトリとflacファイル抽出
	if (dp != NULL) {
		while ((dir = readdir(dp)) != NULL) {
			if (dir->d_name[0] == '.') continue;
			string path = string(dirname) + "/" + string(dir->d_name);
			stat(path.c_str(), &st);
			if (path.length() > 5 && path.substr(path.length() - 5) == ".flac") {
				flacs.push_back(path);
			}
			if ((st.st_mode & S_IFMT) == S_IFDIR) {
				dirs.push_back(path);
			}
		}

		// flacが見つかれば再生 なければ再帰的に処理
		srand(time(NULL));
		if (flacs.size() > 0) {
			cout << "play -> " << flacs[rand()%flacs.size()] << endl;
			string cmd = string(player) + " \"" + flacs[rand()%flacs.size()] + "\"";
			int res = system(cmd.c_str());
		} else if (dirs.size() > 0) {
			play_random_flac(player, dirs[rand()%dirs.size()].c_str());
		} else {
			// ディレクトリ最深部までflacがないのでmain()で再スキャン
			cout << "rescanning..." << endl;
		}
	}
}

int main(int argc, char * argv[]) {
	if (argc < 3) {
		cerr << "useage: ./shuffle_player [player] [music directory]" << endl;
		exit(0);
	}
	while (1) {
		play_random_flac(argv[1], argv[2]);
		sleep(1);
	}
}