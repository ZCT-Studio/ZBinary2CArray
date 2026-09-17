// Licensed under the MIT License

//
// Created by wanjiangzhi on 2026/9/17.
//

#include "ZBinary2CArray/zbtca.h"

int main() {
    ZBTCA_Bin bin(R"(D:\Projects\wanjiangzhi\CLionProjects\ZBinary2CArray\ZBinary2CArray\bin.hpp)");
    ZBTCA_Output output(bin);
    {
        auto& cfg = output.Config();
        using namespace ZBTCA_Types;
        cfg.HeaderOnly = false;
        cfg.ConstSpecifier = OutputCfg::ConstSpecifier_const;
        cfg.StorageSpecifier = OutputCfg::StorageSpecifier_static;
        cfg.ExportTypeFlags = TypeFlags::u64;
    }
    // ReSharper disable once CppExpressionWithoutSideEffects
    output("bin.hpp.测试.cpp");

    return 0;
}