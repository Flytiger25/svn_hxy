#ifndef DRCCS_SVM_H
#define DRCCS_SVM_H

#include <vector>
#include "svm.h"
class Predictor {
private:
    svm_problem* make_svm_problem(const std::vector<int> &label, const std::vector<std::vector<double>> &data);
    void free_svm_problem(svm_problem *prob);
protected:
    size_t data_length;
    struct svm_parameter param;
    struct svm_model *model = nullptr;
public:
    Predictor();
    virtual ~Predictor();

    void train(const std::vector<int> &label, const std::vector<std::vector<double>> &data);
	std::vector<int> predict(const std::vector<std::vector<double>> &data);
    int predict_one(const std::vector<double> &data);
};

#endif //DRCCS_SVM_H
