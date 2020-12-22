# Реализованные эффекты
Мною были реализованы следующие эффекты:

1)Попиксельный расчет освещенности с использованием модели  Блинна-Фонга и гамма-корекцией (гаммма корекцию можно выключить, клавиша T)

2)Cubemap с эффектом окружающей среды

3)Постэффект - инверсия, активация - нажать на кнопку Z на клавиатуре

4)Паралакс с использование relief mapping

5)Карты нормалей.

6)Переменная резкость

# Навигация по сцене
Осуществляется с помощью клавиш WASD и мыши.

# Инструкция по сборке
Проект и библиотеки были собраны с помощью компилятора mingw в среде разработки cline. Компилятор прилагается ввиде архива, с более новой версией mingw мне собрать проект не удалось - библиотеки оказываются несовместимыми. Сборку я проводил только с помощью cline, по-идее проект должен собираться с помощью cmake и без cline. Платформа - winodiws.
