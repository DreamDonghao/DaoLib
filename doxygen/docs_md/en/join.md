@page join Participate in DaoLib Development

# Development Process

Welcome to contributing to the development of DaoLib. This project uses a protected main branch. All changes must be completed via Pull Requests. Please follow the process below:

---

## 1. Fork the Repository

Click “Fork” to copy the repository to your personal GitHub account.

---

## 2. Clone the Repository

Download the code to your local machine:

```bash  
git clone https://github.com/<your-username>/DaoLib.git  
cd DaoLib  
```  

---

## 3. Create a Feature Branch

Do not develop directly on the main branch. You may use your GitHub username as the branch name:

```bash  
git checkout -b feature/your-feature  
```  

---

## 4. Write Code and Commit

```bash  
git add .  
git commit -m "Brief description of the changes"  
git push origin feature/your-feature  
```  

---

## 5. Submit a Pull Request

Go to GitHub and create a Pull Request:

* Select your branch → this repository’s `main` branch
* Fill in the title and description

The PR will automatically trigger the review process.

---

## 6. Review and Revision

Maintainers will review the PR. If adjustments are needed, please continue to commit on the same branch—the PR will update automatically.

---

## 7. Merge

After passing the review, maintainers will merge the PR into the main branch.

---

## 8. Code of Conduct

Please keep discussions polite, clear, and professional.

---

If you have any questions, you may raise them in the Issue section for feedback or discuss them in the Discussions area.

# Project Dependencies

The following are the tested versions that can successfully build the project. You may try other versions at your own discretion.

- C++ 23
- CMake 3.31
- vcpkg :sdl3 3.2.26 sdl3-image 3.2.4[png] <!--For details on third-party libraries, please refer to the vcpkg.json file-->