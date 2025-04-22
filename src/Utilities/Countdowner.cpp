/**
  * @file Countdowner.cpp
  * @brief Implements the global Countdowner instance.
  */
 
  #include "Countdowner.hpp"
 
  /**
   * @brief Allocate the global scheduler.
   *
   * Users can call countdowner->addEvent(...) and countdowner->loop().
   */
  Countdowner* countdowner = new Countdowner();