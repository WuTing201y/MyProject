#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define MAX_COURSES 100
#define DATA_PATH "C:\\Zoey\\My Project\\courses.txt"
//定義課程類型(必修/選修)
typedef enum{ REQUIRED, ELECTIVE } CourseType;

//單一課程的結構
typedef struct{
    char name[50];  //課程名稱
    int credit; //學分數
    CourseType type;    //課程類型
    int is_completed;   //是否已修(0/1)
    int planned_semester; //計畫修讀學期(1~8，0表示未規劃)
}Course;

//學生資訊結構
typedef struct{
    Course* courses;
    int total_courses;
}Student;

Course courses[MAX_COURSES];
int course_count = 0;

void display_menu()
{
    printf("\n===== 學分管理系統 =====\n");
    printf("1. 新增課程\n");
    printf("2. 編輯課程\n");
    printf("3. 顯示所有課程\n");
    printf("4. 統計學分\n");
    printf("5. 保存資料\n");
    printf("6. 讀取資料\n");
    printf("7. 刪除資料\n");
    printf("0. 退出\n");
    printf("請輸入選項：");
}
void edit_courses()
{
    char edit_course[100] = {0};
    printf("請輸入要編輯的課程：");
    scanf("%s", edit_course);
    int found = 0;
    for(int i = 0; i < course_count; i++)
    {
        int user_target;
        if(strcmp(edit_course, courses[i].name) == 0){
            found = 1;
            printf("\n1. 課程名稱\n2. 學分數\n3. 課程類型\n4. 是否已修\n5. 預計修讀學期\n請輸入欲修改項目：");
            scanf("%d", &user_target);

            switch(user_target){
                case 1:
                    printf("\n修改課程名稱為：");
                    scanf("%s", courses[i].name);
                    break;
                case 2:
                    printf("\n修改學分數為：");
                    scanf("%d", &courses[i].credit);
                    break;
                case 3:
                    printf("\n修改課程類型為(0-必修, 1-選修)：");
                    scanf("%u", &courses[i].type);
                    break;
                case 4:
                    printf("\n修改是否已修：");
                    scanf("%d", &courses[i].is_completed);
                    break;
                case 5:
                    printf("\n修改預計修讀學期為(1~8，0表示未規劃)：");
                    scanf("%d", &courses[i].planned_semester);
                    break;
            }
            printf("已成功編輯！");
        }
    }
    if(!found){
        printf("找不到%s課程\n", edit_course);
    }
}
void add_courses()
{
    if(course_count >= MAX_COURSES){
        printf("課程已達上限！\n");
        return;
    }

    Course new_course;
    printf("請輸入課程名稱：");
    scanf("%s", new_course.name); //不需要&，因為陣列本身就是指標
    printf("請輸入課程學分數：");
    scanf("%d", &new_course.credit);
    printf("請輸入課程類型(0-必修, 1-選修)：");
    scanf("%d", (int*)&new_course.type);
    printf("是否已修(0-否, 1-是)：");
    scanf("%d", &new_course.is_completed);
    new_course.planned_semester = 0;

    courses[course_count++] = new_course;
    printf("課程新增成功！\n");
}
void list_courses()
{
    printf("\n===== 課程列表 =====\n");
    for(int i = 0; i < course_count; i++){
        printf("[%d]. %s | 學分：%d | 類型：%s | 狀態：%s\n",
               i+1,
               courses[i].name,
               courses[i].credit,
               courses[i].type == REQUIRED ? "必修" : "選修",
               courses[i].is_completed ? "已修" : "未修"
               );
    }
}
void delete_courses()
{
    char delete_course[100] = {0};
    printf("請輸入要刪除的課程名稱：");
    scanf("%s", delete_course);

    //查找課程
    int found = 0;
    for(int i = 0; i < course_count; i++)
    {
        if(strcmp(delete_course, courses[i].name) == 0){
            /*不能直接寫 delete_course == courses[i].name 這樣是比較指標位址，而不是內容，正確比較兩字串內容必須使用strcmp  */
            /*delete_course 會被視為「指向 delete_course[0] 的指標」courses[i].name 會被視為「指向 courses[i].name[0] 的指標*/

            //找到了，把後面的元素都往前一格
            for(int j = i; j < course_count-1; j++){
                courses[j] = courses[j+1];

            }
            course_count--;
            found = 1;
            printf("%s 課程已成功刪除", delete_course);
            break;
        }
    }
    if(!found){
        printf("找不到%s課程\n", delete_course);
    }

}
void save_data()
{
    FILE* file = fopen(DATA_PATH, "w");
    if(!file){
        printf("無法建立檔案！\n");
        return;
    }
    for(int i = 0; i < course_count; i++){
        fprintf(file, "%s,%d,%d,%d,%d\n",
                courses[i].name,
                courses[i].credit,
                courses[i].type,
                courses[i].is_completed,
                courses[i].planned_semester);
    }
    fclose(file);
    printf("資料已保存！\n");
}
void load_data()
{
    FILE* file = fopen(DATA_PATH, "r");
    if(!file){
        printf("找不到存檔！\n");
        return;
    }

    course_count = 0; //課程計數歸零
    char line[100]; //暫存每行內容

    while(fgets(line, sizeof(line), file)){
        Course new_course;
        sscanf(line, "%[^,],%d,%d,%d,%d",
               new_course.name,
               &new_course.credit,
               (int*)&new_course.type,
               &new_course.is_completed,
               &new_course.planned_semester);
       courses[course_count++] = new_course;
    }
    fclose(file);
    printf("資料載入完成！\n");
}

int main()
{
    SetConsoleCP(65001);         // 設定 Console Input 為 UTF-8
    SetConsoleOutputCP(65001);   // 設定 Console Output 為 UTF-8
    int choice;
    do{
        display_menu();
        scanf("%d", &choice);
        switch(choice){
            case 1: add_courses(); break;
            case 2: edit_courses(); break;
            case 3: list_courses(); break;
            case 5: save_data(); break;
            case 6: load_data(); break;
            case 7: delete_courses(); break;
        }
    }while(choice != 0);

    return 0;
}
