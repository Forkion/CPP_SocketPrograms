#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <condition_variable>
#include <iostream>
#include "../../lib/hdr/lib.hpp"

/**
 * @class app_gen
 * @brief Базовый класс для приложений с сокетным взаимодействием
 * @details Предоставляет общие поля и методы для работы с буфером данных,
 * сокетом и управлением состоянием приложения
 */
class app_gen {
 protected:
  std::string buffer;  ///< Буфер для хранения данных между потоками
  int socket_fd;  ///< Дескриптор сокета (-1 если не подключен)

 public:
  /**
   * @brief Конструктор по умолчанию
   * @details Инициализирует пустой буфер, флаг app_exit = false,
   * socket_fd = -1
   */
  app_gen();

  /**
   * @brief Конструктор с копированием строки
   * @param new_str Строка для инициализации буфера
   * @details Инициализирует буфер переданной строкой
   */
  app_gen(std::string &new_str);

  /**
   * @brief Конструктор с перемещением строки
   * @param new_str Строка для инициализации буфера (rvalue-ссылка)
   * @details Инициализирует буфер перемещением строки
   */
  app_gen(std::string &&new_str);

  /**
   * @brief Деструктор по умолчанию
   */
  ~app_gen() = default;

  /**
   * @brief Устанавливает значение буфера
   * @param new_str Новая строка для буфера
   */
  void set_buffer(const std::string &new_str);

  /**
   * @brief Устанавливает дескриптор сокета
   * @param new_socket_fd Новый дескриптор сокета
   */
  void set_socket_fd(const int &new_socket_fd) noexcept;

  /**
   * @brief Возвращает содержимое буфера
   * @return Копия строки из буфера
   */
  [[nodiscard]] std::string get_buffer() const noexcept;

  /**
   * @brief Возвращает дескриптор сокета
   * @return Дескриптор сокета (-1 если не подключен)
   */
  [[nodiscard]] int get_socket_fd() const noexcept;

  /**
   * @brief Инициализирует ввод строки от пользователя
   * @param str Ссылка на строку для сохранения введенных данных
   * @details Выполняет ввод до 64 символов и проверяет, что ввод содержит
   * только цифры
   */
  void init(std::string &str);
};
