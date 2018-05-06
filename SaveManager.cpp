
#include "SaveManager.h"

SaveManager::SaveManager(int x) {
	currentSave.chapter = x;
	currentSave.offset = 0;
}

SaveManager::~SaveManager() {

}

void SaveManager::Save(TextGrabber& storyReader, TextHandler& text) {
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

	fclose(save);
}

void SaveManager::Load(TextGrabber& storyReader, TextHandler& text) {
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
	
				token = strtok_s(NULL, "=\n", &nextTok);
			}
		}
		fclose(save);
	}
	storyReader.setChOff(currentSave.chapter, currentSave.offset);

}