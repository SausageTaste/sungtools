import os
import shutil


def get_root_path():
    return os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))


def main():
    root_path = get_root_path()
    bundle_path = os.path.join(root_path, "bundle")
    source_path = os.path.join(root_path, "source")
    bundle_include_path = os.path.join(bundle_path, "include")

    for item in os.listdir(source_path):
        item_path = os.path.join(source_path, item)
        if not os.path.isdir(item_path):
            continue

        include_path = os.path.join(item_path, "include")
        shutil.copytree(include_path, bundle_include_path, dirs_exist_ok=True)

    build_source_path = os.path.join(root_path, "build", "source")
    configurations = ["Debug", "Release"]

    for item in os.listdir(build_source_path):
        item_path = os.path.join(build_source_path, item)
        if not os.path.isdir(item_path):
            continue

        for item2 in os.listdir(item_path):
            if item2 not in configurations:
                continue

            config_path = os.path.join(item_path, item2)
            config_output_path = os.path.join(bundle_path, item2)
            os.makedirs(config_output_path, exist_ok=True)

            # copy only lib files
            for item3 in os.listdir(config_path):
                if item3.endswith(".lib"):
                    lib_output_path = os.path.join(config_output_path, "lib")
                    os.makedirs(lib_output_path, exist_ok=True)
                    shutil.copy(os.path.join(config_path, item3), os.path.join(lib_output_path, item3))

    deps_path = os.path.join(root_path, "build", "_deps")
    shutil.copytree(os.path.join(deps_path, "sung_fetch_optional-src", "include"), bundle_include_path, dirs_exist_ok=True)
    shutil.copytree(os.path.join(deps_path, "sung_fetch_expected-src", "include"), bundle_include_path, dirs_exist_ok=True)


if __name__ == "__main__":
    main()
