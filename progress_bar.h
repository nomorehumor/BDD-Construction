//
// Created by Maxim.Popov on 25.06.2022.
//

#ifndef CODE_PROGRESS_BAR_H
#define CODE_PROGRESS_BAR_H


class progress_bar {
public:
    progress_bar(int totalItems);
    void update(int itemsDone);
private:
    int totalItems;
};


#endif //CODE_PROGRESS_BAR_H
