#ifndef _H_REPOSITORY_H
#define _H_REPOSITORY_H

#include <vector>
#include <memory>
#include <algorithm>
#include <concepts>
#include <mutex>

template <typename T>
concept Identifiable = requires (T a) {
    { a.getId() } -> std::convertible_to<int>;
};

template <Identifiable T>
class Repository {
    public:
        void add(std::shared_ptr<T> item);
        std::vector<std::shared_ptr<T>> getAll() const;
        std::shared_ptr<T> findById(int id) const;
        void removeById(int id);
        std::shared_ptr<T> operator[](int index) const;

        template <typename Predicate>
        std::vector<std::shared_ptr<T>> filter(Predicate pred) const {
            std::lock_guard<std::mutex> lock(mtx);
            std::vector<std::shared_ptr<T>> result;
            for (const auto& item : items) {
                if (pred(item)) result.push_back(item);
            }
            return result;
        }

    private:
        std::vector<std::shared_ptr<T>> items;
        mutable std::mutex mtx;
};


template <Identifiable T>
std::vector<std::shared_ptr<T>> Repository<T>::getAll() const {
    std::lock_guard<std::mutex> lock(mtx);
    return items;
}

template <Identifiable T>
void Repository<T>::add(std::shared_ptr<T> item) {
    std::lock_guard<std::mutex> lock(mtx);
    items.push_back(item);
}

template <Identifiable T>
std::shared_ptr<T> Repository<T>::findById(int id) const {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& item : items) {
        if (item->getId() == id) {
            return item;
        }
    }
    return nullptr;
}

template <Identifiable T>
void Repository<T>::removeById(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    items.erase(std::remove_if(items.begin(), items.end(),
        [id](const std::shared_ptr<T>& item) { return item->getId() == id; }), items.end());
}

template <Identifiable T>
std::shared_ptr<T> Repository<T>::operator[](int index) const {
    std::lock_guard<std::mutex> lock(mtx);
    if (index >= 0 && index < static_cast<int>(items.size())) {
        return items[index];
    }
    return nullptr;
}

#endif // _H_REPOSITORY_H