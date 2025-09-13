# Интегрирование методом Монте-Карло

Проект предполагает исследование производительности программы на синтетических тестах с использованием параллельных вычислений

## Конфигурация Release сборки
Конфигурации для `Release` и `Debug` сборок должны находится в разных поддиректориях.
Иначе сбой в сборке

```bash
    cmake -G "Unix Makefiles" -S . -B build/Release -DCMAKE_BUILD_TYPE=Release
```

## Конфигурация Debug сборки

```bash
    cmake -G "Unix Makefiles" -S . -B build/Debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang
```

## Сборка с нуля

`--verbose` - для полного вывода всех исполняемых команд
`--clean-first` - для чистой сборки из исходников

```bash
    cmake --build build/Release/ -j20 --verbose --clean-first
```

## Генерация документации

