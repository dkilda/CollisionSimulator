#include <iostream>
#include <SFML/Graphics.hpp>

#include "sfml_util.h"

//helpers for SFML

void check_user_events(sf::RenderWindow &window)
{  //check for user events like mouse && button clicks
   sf::Event event;
   while (window.pollEvent(event)) {
       if ((event.type == sf::Event::Closed) ||
          ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
          window.close();
          break;
       }
   }
}
