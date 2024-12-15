#include "stdafx.h"
#include "Predictor.h"
#include <cstdlib>
#include <stdexcept>
#include <iostream>

Predictor::Predictor() {
    param.svm_type = C_SVC;
    param.kernel_type = POLY;
    param.degree = 3;
    param.gamma = 1;	// 1/num_features
    param.coef0 = 0.0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 1;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 0;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
}

Predictor::~Predictor() {

}

void Predictor::train(const std::vector<int> &label, const std::vector<std::vector<double>> &data) {
    if(label.size() != data.size()) {
        throw std::runtime_error("Label and data size not equal.");
    }


    this->data_length = data.front().size();
    auto prob = make_svm_problem(label, data);

    if(this->model != nullptr) {
        svm_free_and_destroy_model(&(this->model));
    }
    this->model = svm_train(prob, &this->param);
    free_svm_problem(prob);
}

std::vector<int> Predictor::predict(const std::vector<std::vector<double>> &data) {
	std::vector<int> r;
	int val;
	for (int i = 0; i < data.size(); i++)
	{
		val = predict_one(data[i]);
		r.push_back(val);
	}
	return r;
}

int Predictor::predict_one(const std::vector<double> &data) {
    if(this->model == nullptr) {
        throw std::runtime_error("Model not trained.");
    }

    if(data.size() != this->data_length) {
        throw std::runtime_error("Data vector length mismatch.");
    }

    auto x = new svm_node[data.size()+1];
    for(int i = 0; i < this->data_length; i++) {
        x[i].index = i;
        x[i].value = data[i];
    }
    x[this->data_length].index = -1;
    x[this->data_length].value = 0;

    double result = svm_predict(this->model, x);

	delete[] x;

    return int(result+1e-6);
}

svm_problem* Predictor::make_svm_problem(const std::vector<int> &label, const std::vector<std::vector<double>> &data) {
    size_t size = label.size();
    size_t length = data.front().size();

    auto *p = new svm_problem;
    p->l = size;

    p->y = new double[length];
    for(int i = 0; i < size; i++) {
        p->y[i] = label[i];
    }

    p->x = new svm_node*[size];
    for(int i = 0; i < size; i++) {
        p->x[i] = new svm_node[length+1];
        for(int j = 0; j < length; j++) {
            p->x[i][j].index = j;
            p->x[i][j].value = data[i][j];
        }
        p->x[i][length].index = -1;
        p->x[i][length].value = 0;
    }
    return p;
}

void Predictor::free_svm_problem(svm_problem *prob) {
    // TODO: svm_problem memory leakage.
    // The line avove causes crash.
    //delete[] prob->y;
    for(int i = 0; i < prob->l; i++) {
        delete[] prob->x[i];
    }
    delete[] prob->x;
    delete prob;
}