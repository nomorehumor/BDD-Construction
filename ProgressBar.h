//
// Created by Maxim.Popov on 25.06.2022.
//

#ifndef CODE_PROGRESSBAR_H
#define CODE_PROGRESSBAR_H


class ProgressBar {
public:
    ProgressBar(int totalItems);
    void update(int itemsDone);
private:
    int totalItems;
};


#endif //CODE_PROGRESSBAR_H
