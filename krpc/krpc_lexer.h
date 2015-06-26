#ifndef KRPC_LEXER_H
#define KRPC_LEXER_H

#include <exception>
#include <string>
#include <list>

class krpc_token_t;
class krpc_lexer_t;

/*
 * token类型
 */
enum {
    krpc_token_i8 = 1,
    krpc_token_i16,
    krpc_token_i32,
    krpc_token_i64,
    krpc_token_ui8,
    krpc_token_ui16,
    krpc_token_ui32,
    krpc_token_ui64,
    krpc_token_f32,
    krpc_token_f64,
    krpc_token_string,
    krpc_token_array,
    krpc_token_object,
    krpc_token_rpc,
    krpc_token_import,
    krpc_token_text,
    krpc_token_left_brace,
    krpc_token_right_brace,
    krpc_token_left_round,
    krpc_token_right_round,
    krpc_token_comma,
};

/*
 * token
 */
class krpc_token_t {
public:
    /*
     * 构造函数
     * @param literal 字面量
     * @param row 行
     * @param col 列
     * @param type 类型
     */
    krpc_token_t(const std::string& literal, int row, int col, int type);

    /*
     * 析构
     */
    ~krpc_token_t();

    /*
     * 取得token字面量
     * @return token字面量
     */
    const std::string& get_literal() const;

    /*
     * 取得行
     */
    int get_row() const;

    /*
     * 取得列
     */
    int get_col() const;

    /*
     * 取得类型
     */
    int get_type() const;

private:
    std::string _literal; // 字面量
    int         _row;     // 行
    int         _col;     // 列
    int         _type;    // 类型
};

/*
 * token解析器
 */
class krpc_lexer_t {
public:
    /*
     * 构造
     * @param stream 字符流
     */
    krpc_lexer_t(const char* stream);

    /*
     * 析构
     */
    ~krpc_lexer_t();

    /*
     * 取得下一个token
     * @return krpc_token_t实例
     */
    krpc_token_t* next_token() throw(std::exception);

    /*
     * 取得当前行
     */
    int get_col();

    /*
     * 取得当前列
     */
    int get_row();

private:
    /*
     * 取得下一个token
     * @return krpc_token_t实例
     */
    krpc_token_t* get_token();

    /*
     * 消除流当前空白
     */
    void eat_whites();

    /*
     * 消除流当前注释
     */
    void eat_comment();

    /*
     * 检查是否为空白
     * @param c 当前字符
     * @retval true 是
     * @retval false 否
     */
    bool check_white(char c);

    /*
     * 检查是否为终结符
     * @param c 当前字符
     * @retval true 是
     * @retval false 否
     */
    bool check_terminator(char c);

    /*
     * 步进流
     * @param step 步长
     * @return 当前字符
     */
    char forward(int step);

    /*
     * 增加列
     * @param cols 列
     */
    void add_col(int cols);

    /*
     * 递增行
     */
    void inc_row();

    /*
     * 检查流是否有效
     */
    bool verify();

    /*
     * 取得当前字符
     */
    char current();

    /*
     * 流是否结束
     */
    bool has_next();

    /*
     * 检查变量名是否合法
     * @param var_name 变量名
     * @retval true 是
     * @retval false 否
     */
    bool check_var_name(const char* var_name);

    //
    // 关键字比较
    //

    bool check_keyword2(const char* s, char c1, char c2);
    bool check_keyword3(const char* s, char c1, char c2, char c3);
    bool check_keyword4(const char* s, char c1, char c2, char c3, char c4);
    bool check_keyword5(const char* s, char c1, char c2, char c3, char c4, char c5);
    bool check_keyword6(const char* s, char c1, char c2, char c3, char c4, char c5, char c6);

private:
    typedef std::list<krpc_token_t*> token_list_t;
    token_list_t _tokens;  // token链表
    int          _row;     // 当前行
    int          _col;     // 当前列
    char*        _stream;  // 字符流
};

#endif // KRPC_LEXER_H
