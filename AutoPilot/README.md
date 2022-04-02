# AutoPilot

新增头文件RobustMatcher.h。
修改文件Model.cpp中的surf，加入了加权滤除。
main.cpp为示例使用RobustMatcher.h的match来进行偏移x,y,center的计算（已集成Model.cpp的surf)。
center误差阈值：只要两幅图正反输入有绝对值小于0.02的center都舍去，若两图正反输入绝对值均大于0.18，则有效。
x,y误差+-0.01
