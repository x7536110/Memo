# Memo
一个备忘录小工具，基于wxWidgets3.0开发  
# 测试版本 0.2.1

* 功能&使用说明
 * 在文本框中输入待办事项，点击添加按钮添加到备忘录
 * 可以标记已完成的事项
 * 可以删除已被标记完成的事项
 * 关闭程序自动保存日志，下次启动自动加载

# 现存bug:
 * 不支持新windows DPI 缩放机制，高缩放比率下出现排版混乱现象
 * Linux系统下对Unicode支持存在bug，无法显示中文字符

## 更新日志
  * 测试版本0.2.1  
   * 添加LOGO
  * 测试版本0.2 
	* 删除了“不可使用空格等字符”的限制
  * 测试版本0.1 
	* 修正了保存与加载功能
	* 修正了windows下的中文支持
    
	
##开发环境：

* WxWidgets 3.0.2   
* GCC4.8.1(TDM64-2)

###Wxwidgets：
[点此下载](https://github.com/wxWidgets/wxWidgets)
