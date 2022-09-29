BUILD_DIR := build


TEST_CASES := \
	test_base64 


# 忽略的路径
IGNORE_PATHS := \
    */build \
    */build/* \
    */archive \
    */archive/* \
    */tmp \
    */tmp/* \
    *.tmp \
    *.tmp/* \
    */old \
    */old/* \
    *.old \
    *.old/* \
    *.bak \
    *.bak/* \
    */temp \
    */temp/* \
    */backup \
    */backup/* 

# 额外忽略log目录
IGNORE_PATHS += \
    */log \
    */log/* 

EXTRA_FIND_ARGS := $(foreach d, $(IGNORE_PATHS), ! -path "$(d)")

# 源码目录
SRCDIRS	:= \
    $(shell find . -type d $(EXTRA_FIND_ARGS)) 

# 头文件搜索目录
INCDIRS := \
    . \
    $(SRCDIRS)

INC := $(addprefix -I, $(INCDIRS))


vpath %.c $(SRCDIRS)
vpath %.cpp $(SRCDIRS)


# 默认目标
no_target:
	@echo "Please provide the test case name. eg: make test_iconv_util"

all: $(TEST_CASES)


$(BUILD_DIR):
	@-mkdir -p $@


test_base64: test_base64.c base64.c base64_ex.c | $(BUILD_DIR)
	gcc -o $(BUILD_DIR)/$@ $^ $(INC)
	mkdir -p ./tmp && $(BUILD_DIR)/$@


.PHONY: no_target all $(TEST_CASES)
