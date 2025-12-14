# ENG:

## Brain Train

The project was carried out as part of the Creative Radio Electronics supplementary general education program at the Academy of Digital Technologies (2023–2024).

**Brain Train** is an interactive memory trainer based on *Arduino Nano*, designed to train attention, short-term memory, and accuracy. During the test, the user observes randomly selected $n$ LEDs that start to light up. Then, they must reproduce this sequence by pressing the corresponding blue buttons (*S*). The red control buttons (*MS*) are used to start, cancel, give hints, and reset.

Next, we will discuss $n=6$ LEDs

The project helps develop and improve cognitive functions, such as the ability to quickly memorize visual information and accurately reproduce it, which is useful for anyone who wants to maintain or improve their memory and concentration.

* When turned on, a start message appears on the display.

* Pressing *MS1* starts the countdown (for 3 seconds).

* Then, 6 randomly selected *LED* LEDs start to light up.

* After the lights turn off, the user reproduces the sequence by pressing the corresponding *S* buttons.

* *MS3* cancels the last press.

* *MS4* shows a hint or the result if the user pressed the correct number of buttons.

* *MS2* resets the current progress and returns to the start screen.

* The result is displayed as a percentage of matches and the number of correct answers.

The main logic of the test is implemented in the `main.ino` file.  
Additional modules for debugging and visualization:

- `readButtonData.ino` — reads and outputs information about the pressed buttons (*S*) to Serial

- `displayButtonData.ino` — displays information about button presses in real time on the *OLED* display.

- `showLEDs.ino` — responsible for generating and displaying random *LED* patterns.

Key parameters can be configured in the code:

- `n` — number of *LED* lights that light up
- `beforeStartInterval` - countdown time before start
- `showLEDsInterval` - duration of *LED* illumination
- `showResultInterval` - message display time

Regular training with **Brain Train** helps:

- Improve concentration and attention span  
- Increase the speed of visual information processing  
- Develop short-term memory  
- Maintain brain activity and cognitive functions in good shape  

This project is ideal for those who want to maintain mental acuity, as well as older people who care about preserving their memory.

Author: Robert Bitlev

Mentor: P.N. Romanko

---

The **Brain Train** project is an educational and entertainment project and is not intended for the diagnosis, treatment, or prevention of any disease. Use of the device does not replace consultation with a doctor or specialist. Before starting any training, especially if you have medical contraindications, it is recommended that you consult a qualified specialist. The author of the project is not responsible for any consequences associated with the use of this trainer.

# RU:

## Brain Train

Проект был выполнен в рамках обучения по дополнительной общеразвивающей программе «Творческая радиоэлектроника», проходившей в Академии Цифровых Технологий (2023 - 2024 гг.)

**Brain Train** — интерактивный тренажёр памяти на основе *Arduino Nano*, предназначенный для тренировки внимания, кратковременной памяти и точности. В ходе теста пользователь наблюдает за случайно выбранными $n$ светодиодами *LED*, которые начинают светиться. Затем необходимо воспроизвести эту последовательность, нажимая соответствующие синие кнопки (*S*). Красные кнопки управления (*MS*) служат для запуска, отмены, подсказок и сброса.

Далее речь будет идти о $n=6$ светодиодах

Проект помогает развивать и улучшать когнитивные функции, такие как способность быстро запоминать визуальную информацию и точно воспроизводить её, что полезно для всех, кто хочет поддерживать или улучшать память и концентрацию.

* При включении на дисплее появляется стартовое сообщение

* Нажатие *MS1* запускает обратный отсчёт (на 3 секунды)

* Затем случайно выбранные 6 светодиодов *LED* начинают светиться

* После окончания свечения пользователь воспроизводит последовательность, нажимая соответствующие кнопки *S*

* *MS3* отменяет последнее нажатие

* *MS4* показывает подсказку или итог, если пользователь нажал правильное количество кнопок

* *MS2* сбрасывает текущий прогресс и возвращает к стартовому экрану

* Итог выводится в процентах совпадений и количестве правильных ответов.

Основная логика теста реализована в файле `main.ino`.  
Дополнительные модули для отладки и визуализации:

- `readButtonData.ino` — считывает и выводит в Serial информацию о нажатых кнопках (*S*)

- `displayButtonData.ino` — отображает на *OLED*-дисплее информацию о нажатиях в реальном времени.

- `showLEDs.ino` — отвечает за генерацию и показ случайных шаблонов *LED*.

В коде можно настроить ключевые параметры:

- `n` - количество загорающихся светодиодов *LED*
- `beforeStartInterval` - время обратного отсчёта перед началом
- `showLEDsInterval` - длительность свечения *LED*
- `showResultInterval` - время отображения сообщений

Регулярные тренировки с помощью **Brain Train** помогают:

- Улучшить способность к концентрации и вниманию  
- Повысить скорость обработки зрительной информации  
- Развить кратковременную память  
- Поддерживать мозговую активность и когнитивные функции в тонусе  

Этот проект идеально подходит для тех, кто хочет поддерживать остроту ума, а также пожилых людей, заботящихся о сохранении памяти.

Автор: Битлев Роберт

Наставник: Романько П.Н.

---

Проект **Brain Train** является образовательным и развлекательным проектом и не предназначен для диагностики, лечения или профилактики каких-либо заболеваний. Использование устройства не заменяет консультацию с врачом или специалистом. Перед началом любых тренингов, особенно при наличии медицинских противопоказаний, рекомендуется проконсультироваться с квалифицированным специалистом. Автор проекта не несёт ответственности за возможные последствия, связанные с использованием этого тренажёра.
