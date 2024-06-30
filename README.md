# Alemdar

Alemdar is fast, basic neural network library. It's designed for experimental usages, not recommended to use commercial.

## Getting Started

These instructions will give you a copy of the project up and running on
your local machine for development and testing purposes. See deployment
for notes on deploying the project on a live system.

### Prerequisites

Requirements for the software and other tools to build, test and push

- [libpng](http://www.libpng.org/pub/png/libpng.html)
- [libjpeg](http://www.ijg.org/)

### Installing

A step by step series of examples that tell you how to get a development
environment running

Clone project

    git clone https://github.com/omerfdmrl/alemdar alemdar

Run example network

    cd alemdar
    cp ./examples/main.c ./main.c
    make

End with an example of training of AND gate.

### Usage

You can check `examples/` folder until I wrote doc.

### Roadmap

- [x] Initializers
  - [x] Ones
  - [x] Random HeUniform
  - [x] Random Normal
  - [x] Random Uniform
  - [x] Random Xavier
  - [x] Zeros
- [x] Activation
  - [x] Sigmoid
  - [x] Tanh
  - [x] Softmax
- [ ] Layers
  - [x] Activation
  - [x] Dense
  - [ ] Dropout (exists, but have problems)
  - [x] Shuffle
  - [x] Max Pooling
  - [x] Min Pooling
  - [x] Mean Pooling
  - [ ] Flatten
  - [ ] RNN (still testing)
  - [ ] GRU (under development)
- [ ] Models
  - [ ] Sequential
  - [ ] GAN
- [ ] Optimizers
  - [x] Finite Diff
  - [ ] Gradient Descent
    - [x] Batch Gradient Descent
    - [ ] Stochastic Gradient Descent
    - [ ] Mini-Batch Gradient Descent
- [x] Utils
  - [x] CSV
  - [x] IMG
  - [x] IO
- [ ] TODO
  - [ ] Function for split test-train data
  - [ ] 3D Layers
  - [ ] 3D Models

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".

## Versioning

We use [Semantic Versioning](http://semver.org/) for versioning. For the versions
available, see the [tags on this
repository](https://github.com/omerfdmrl/alemdar/tags).

## Authors

- **Ömer Faruk Demirel** - _Main Developer_ -
  [omerfdmrl](https://github.com/omerfdmrl)
- **Billie Thompson** - _Provided README Template_ -
  [PurpleBooth](https://github.com/PurpleBooth)

See also the list of
[contributors](https://github.com/omerfdmrl/alemdar/contributors)
who participated in this project.

## License

This project is licensed under the [MIT](LICENSE.md)
Creative Commons License - see the [LICENSE.md](LICENSE.md) file for
details
