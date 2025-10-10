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

```bash
    cd ./docs && doxygen && cd ../
```

Документация будет находится в `docs`. В директории `html` будет html версия, а в `latex` - версия в latex

### Генерация pdf

Чтобы сгенерировать `pdf` необходимо установить пакеты

+ texlive-latex-base texlive-latex-extra texlive-fonts-recommended
+ texlive-meta

Далее выполнить команду

```bash
    cd ./docs/latex && make
```

### Graphviz дамп оптимизированной функции

Чтобы включить дамп оптимизированной функции подайте флаг `--dump-ast` и укажите имя `.dot` файла.
Установите `graphviz` на свой компьютер

После этого выполните команду:

```bash
    dot -Tpng ./filename.dot > img.png
```