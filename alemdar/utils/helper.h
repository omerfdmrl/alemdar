#ifndef ALEMDAR_HELPER_H

#define ALEMDAR_HELPER_H

ALEMDAR_DEF void print_progress(size_t count, size_t max, float cost);
ALEMDAR_DEF void print_progress_header(size_t epoch, size_t total_epochs, float cost);
ALEMDAR_DEF void print_progress_footer(time_t timer, float cost);
int oldProgress = -1;

void print_progress_header(size_t epoch, size_t total_epochs, float cost) {
    printf("\n----------------------------------------------------------------------------------------------------------------------\n");
    printf("Epoch %zu/%zu - Cost = %.12f\n", epoch + 1, total_epochs, cost);
    printf("----------------------------------------------------------------------------------------------------------------------\n");
}

void print_progress(size_t count, size_t max, float cost) {
    float progress = (float)count / max;
    if(oldProgress == (int)(progress * 100)) return;
    oldProgress = (int)(progress * 100);
    int barWidth = 70;
    printf("\rProgress [");
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %d%% Complete - Cost = %.12f", (int)(progress * 100.0), cost);
    fflush(stdout);
    if (count == max) {
        printf("\n\n");
    }
}

void print_progress_footer(time_t timer, float cost) {
    double time_taken = ((double)timer)/CLOCKS_PER_SEC;
    printf("\n\n----------------------------------------------------------------------------------------------------------------------\n");
    printf("Time Taken = %f - Cost = %.12f\n", time_taken, cost);
    printf("----------------------------------------------------------------------------------------------------------------------\n");
}

#endif // !ALEMDAR_HELPER_H
