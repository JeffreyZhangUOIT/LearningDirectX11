#include "SaveManager.h"
using namespace std;

SaveManager::SaveManager(int x) {
	currentSave.chapter = x;
	currentSave.offset = 0;
}

SaveManager::~SaveManager() {

}

void SaveManager::Save(TextGrabber& storyReader, TextHandler& text, PoseArtManager& pose, Background& bg) {
	fopen_s(&save, "save", "w+");
	char buf[256];
	char temp[256];

	strcpy_s(buf, sizeof(buf), "Chapter=");
	sprintf_s(temp, sizeof(temp), "%d", storyReader.chapter);
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "Offset=");
	sprintf_s(temp, sizeof(temp), "%d", (storyReader.offset - 1));
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "Academy=");
	sprintf_s(temp, sizeof(temp), "%s", text.schoolName);
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "MC=");
	sprintf_s(temp, sizeof(temp), "%s", text.mcName);
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "Background=");
	sprintf_s(temp, sizeof(temp), "%d", bg.currentBackground);
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "PoseLeft=");
	sprintf_s(temp, sizeof(temp), "%d", pose.q1 * 10);
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "PoseRight=");
	sprintf_s(temp, sizeof(temp), "%d", pose.q2 * 10);
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "Mifune=");
	sprintf_s(temp, sizeof(temp), "%d", storyReader.Mifune);
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "Akari=");
	sprintf_s(temp, sizeof(temp), "%d", storyReader.Akari);
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "Inori=");
	sprintf_s(temp, sizeof(temp), "%d", storyReader.Inori);
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "Tojihime=");
	sprintf_s(temp, sizeof(temp), "%d", storyReader.Tojihime);
	strcat_s(buf, 256, temp);
	fprintf(save, "%s\n", buf);



	fclose(save);
}

void SaveManager::Load(TextGrabber& storyReader, TextHandler& text, PoseArtManager& pose, Transition& transistor, Timer& time) {
	fopen_s(&save, "save", "r+");
	if (save != NULL) {
		char buffer[256];
		char * token = NULL;
		char * nextTok = NULL;

		while (fgets(buffer, 256, save) != NULL) {
			token = strtok_s(buffer, "=\n", &nextTok);
			while (token != NULL) {
				if (strncmp(token, "Chapter", strlen("Chapter")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					currentSave.chapter = std::stoi(token);
				}
				if (strncmp(token, "Offset", strlen("Offset")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					currentSave.offset = std::stoi(token);
				}
				if (strncmp(token, "Academy", strlen("Academy")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					memcpy(text.schoolName, token, strlen(token));
				}
				if (strncmp(token, "MC", strlen("MC")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					memcpy(text.mcName, token, strlen(token));
				}
				if (strncmp(token, "Background", strlen("Background")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					transistor.init(stoi(token), time);
				}
				if (strncmp(token, "PoseLeft", strlen("PoseLeft")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					pose.enter(stoi(token));
				}
				if (strncmp(token, "PoseRight", strlen("PoseRight")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					pose.enter(stoi(token));
				}
				if (strncmp(token, "Mifune", strlen("Mifune")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					storyReader.Mifune = stoi(token);
				}
				if (strncmp(token, "Akari", strlen("Akari")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					storyReader.Akari = stoi(token);
				}
				if (strncmp(token, "Tojihime", strlen("Tojihime")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					storyReader.Tojihime = stoi(token);
				}
				if (strncmp(token, "Inori", strlen("Inori")) == 0) {
					token = strtok_s(NULL, "=\n", &nextTok);
					storyReader.Inori = stoi(token);
				}
	
				token = strtok_s(NULL, "=\n", &nextTok);
			}
		}
		fclose(save);
	}
	storyReader.setChOff(currentSave.chapter, currentSave.offset);

}

