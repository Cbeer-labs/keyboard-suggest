

### How to start

Для начала склонируйте репозиторий со всеми зависимостями. Для этого выполните в командной строке
```shell script
git clone https://github.com/drewxa/bugs-world.git bugs-world-contest
cd bugs-world-contest
git submodule update --init --recursive
```


### Тестовый запуск
Скомпилируйте тестовый проект:
```shell script
cmake -B _builds . -DBUILD_SSL=NO
cmake --build _builds --target app
```

Запустить приложение:
```shell script
./_builds/app -r ./misc/viewer/
```

В консоли вы увидите адрес, по которому можно перейти наблюдать за процессом, например, 
 http://localhost:1984/viewer/index.html
 
Остановить приложение можно нажав на **Ctrl+C**.

### Как реплизовать своего "бота"

Рассмотрим простой пример - бота, который случайно решает куда пойти.
```c++
struct RandomBug : public Bug {
  ActionType Action(const WorldMap& map) override {
    int i = rand() % 5;
    return static_cast<ActionType>(i);
  }

  Cell::Ptr Clone() const override final { return std::make_unique<RandomBug>(*this); }
};
```

Необходимо реализовать класс, который наследуется от класса `Bug`, объявленного в `"details/bug.n"`.

На каждом новом такте мира вызывается метод `Action` объектов, поэтому вся логика вашего бота должна быть реализована тут.
Именно этот метод определяет, куда пойдет ваш жук. На вход методу поступает часть мира, двумерный массив 3x3.
В координатах `map[1][1]` находится сам жук. Заметим, что передается копия мира, т.е. вы не сможете повлиять на внешний мир.

Кроме этого вам необходимо реализовать метод `Clone`.
В общем случае для этого метода ваш класс должен быть копируемым. Тогда метод `Clone` будет выглядеть следующим образом:
```c++
Cell::Ptr Clone() const override final {
  return std::make_unique<YourTypeName>(*this);
}
```
Вам только необходимо заменить `YourTypeName` на название вашего класса.

