# Simple-Logic-Game---Burger-Starvation readme eng
introduction:
Game was created using Visual Studio 2015 Express, C++ and SFML framework. It was created by myself alone, I started creating it during Slavic Game Jam 2017 and finished it some time after. My goal was to create working game, to learn about creating games from programmer perspective and learn basics of SFML framework. Code is far from perfect but it meets jam requirements - it works, although at some point it was obvious that more abstract programming would be useful - especialy in case of bHandlePlayAgain or vResetGame functions. The thing I learned is that I should pack any kind of functionality in separate a function at least even if I'm not using Object Oriented Programming principles.

---------------------------------------------------------------------------

rules:
The goal is to deplete "feed" meter while keeping "poison" and "turns" meters higher than 0. You can achieve that by first trying which ingredient is "poisoned" - they deplete "poison" and "feed" meter by 1 after adding them and show purple dot near given ingredient. It is safe to use "reset" function when you get "poison" meter to 1. Repeat until  you've discovered all "poisoned" ingredients. There are always 3 "poisoned" ingredients and 3 not "poisoned". After that you can safely deplete "feed" meter with various ingredients - repeating any makes them 2 times less effective. It is important not to waste "turns" since you cannot get them back.

There are 3 meters in game:
purple - "poison" meter, it depletes when you add "poisoned" ingredient into burger. When you add the same "poisoned" ingredient multiple times without using "reset" option it's poisonous effect is halved 2 times each time. Meter resets after using "reset" option. If you reach 0, you've lost.

light blue - "turns" meter, adding any ingredient always depletes it by 1. It does NOT reset after using "reset" option. If you reach 0, you've lost.

green - "feed" meter, it depletes when you add any ingredient, even "poisoned" one. Repeating the same ingredient without using "reset" option halves "feed" value of given ingredient. This meter resets if you use "reset" option. If you reach 0, you've won.

And there are reset button and dots near ingredients:
reset button - it "resets" chosen aspects of game: "feed" meter, "poison" meter, "poison" and "feed" values back to default (1) and visual ingredients in the burger. It does not reset "turns" meter neither dots near ingredients that show if given one is "poisoned" or not.

dots near ingredients - they show if given ingredient is "poisoned" or not. Purple dot means it is, light blue means it isn't. Although you have to discover them first. They do not reset after using "reset" option.

Escape button - pauses the game and gives option to retry or exit it. If game is paused it resumes it.

----------------------------------------------------------------------------

"plot":
You're starving scientist that needs to eat something right now. But you've poisoned half of ingredients you have at home for your experiments, but since jam theme was "unknown" you've forgot which one you have poisoned so you need to eat big enough burger without first starving or dying to poison.

