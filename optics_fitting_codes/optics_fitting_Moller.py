#Author: Vassu Doomra, Stony Brook University

import math
import time
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from colorama import init, Fore, Style
from scipy.optimize import curve_fit
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import PolynomialFeatures, MinMaxScaler
from sklearn import linear_model
import random

def gaussian( x, mu, std):
  return (1 / (std * np.sqrt(2 * np.pi))) * np.exp(-((x - mu) ** 2) / (2 * std** 2))

def PolynomialRegression( X, y, degree, variable):  

  X_train,X_test,y_train,y_test=train_test_split(X, y, test_size=0.33, random_state=42)

  poly = PolynomialFeatures(degree)

  X_train_new=poly.fit_transform(X_train)
  X_test_new=poly.fit_transform(X_test)

  regression = linear_model.LinearRegression()
  model = regression.fit(X_train_new, y_train)
  y_pred_test = regression.predict(X_test_new)

  y_res_test = y_test - y_pred_test

  par = model.coef_
  intercept = model.intercept_

  print(Fore.RED + "The Fit Variable: " + Style.RESET_ALL)
  print(Fore.GREEN + variable + Style.RESET_ALL)

  print("                            ")
  score = model.score(X_test_new, y_test)
  print("score:  ", score)
  print("                            ")
        
  n=len(par[0])
  parameters = np.zeros(n)
  for i in range(n):
    if i==0:
      parameters[i] = intercept[i]
    else:
      parameters[i] =  par[0][i]

  print(Fore.RED + "The Fit Parameters are: " + Style.RESET_ALL)
  print(parameters)
  print("                            ")

  n_bootstrap = 500  # Choose an appropriate number of bootstrap samples
  intercept_samples = []
  coeff_samples = []

  for _ in range(n_bootstrap):

    indices = np.random.choice(len(X_train_new), len(X_train_new), replace=True)
    X_resampled = X_train_new[indices]
    y_resampled = y_train[indices]

    model = linear_model.LinearRegression()
    model.fit(X_resampled, y_resampled)

    # Store the parameters
    intercept_samples.append( model.intercept_)
    coeff_samples.append( model.coef_)

  intercept_samples = np.array(intercept_samples)
  coeff_samples = np.array(coeff_samples)
  intercept_uncertainties = np.std(intercept_samples, ddof=1)
  coeff_uncertainties = np.std(coeff_samples, axis=0, ddof=1)

  print(Fore.RED + "The uncertainties in the fit parameters are: " + Style.RESET_ALL)
  print(intercept_uncertainties, coeff_uncertainties[0][1:])
  print("                            ")

  varNms = ["GEM r [mm]", "GEM rp", "GEM phi [rad]", "GEM phip"] 

  fig1, bx = plt.subplots(1,4)
  fig1.canvas.manager.set_window_title(variable)
 
  for i in range(4):

    bx[i].scatter(X_test[:,[i]],y_test, s=3)
    bx[i].scatter(X_test[:,[i]],y_pred_test, s=3)
    bx[i].set_ylabel(variable)
    bx[i].set_xlabel(varNms[i])
       
  plt.show()

  hist, bin_edges = np.histogram(y_res_test*1000, bins=500, density=True)
  bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
  initial_guess = [np.mean(y_res_test*1000), np.std(y_res_test*1000)]
  params, covariance = curve_fit(gaussian, bin_centers, hist, p0=initial_guess)
  mu, sigma = params

  x = np.linspace(min(bin_centers), max(bin_centers), 500)
  fitted_curve = gaussian(x, mu, sigma)
  plt.hist(y_res_test*1000, bins=500, density=True, alpha=0.7, label='Histogram')
  plt.plot(x, fitted_curve, 'r-', label='Fitted Gaussian')
  plt.xlim(-500,500)
  plt.xlabel(r'$\phi_{lab}^{True} - \phi_{lab}^{Pred}$ [mrad]', fontsize=14)
  plt.ylabel('Counts [Arbitrary Units]', fontsize=14)
  plt.legend()
  plt.text(0.05, 0.9, f'Mean = {mu:.6f} mrad', transform=plt.gca().transAxes, fontsize=12, color='blue')
  plt.text(0.05, 0.85, f'St. Dev. = {sigma:.6f} mrad', transform=plt.gca().transAxes, fontsize=12, color='blue')
  plt.show()

  return parameters

if __name__=='__main__':

  random.seed(int(time.time()))

  cmap = plt.get_cmap('tab10')

  all_hole = [["11","12", "13", "21", "22", "23", "31", "32", "33", "41", "42", "43", "51", "52", "53", "61", "62", "63", "71", "72", "73"],
        ["11", "12", "13", "21", "22", "23", "31", "32", "33", "41", "42", "43", "51", "52", "53", "61", "62", "63", "71", "72", "73"]]
  all_pass = [["p1", "p2", "p3", "p4"], ["p1", "p2", "p3", "p4"]]
  all_target = [["opticsUS","opticsUM", "opticsMS","opticsMD", "opticsDS"], ["opticsDS"]]
    
  fitFlag   = [0, 1]
  varNms    = ['theta [rad]', 'phi [rad]']
  fitDeg    = [2, 2]

  for i in range(2):
    if not fitFlag[i]:
      continue
        
    all_df = pd.DataFrame()

    for a_pass in all_pass[i]:
      for a_hole in all_hole[i]:
        for a_target in all_target[i]:
          file_new = "csv_output/" + str(a_target) + "_" + str(a_pass) + "/C12_" + str(a_target) + "_" + str(a_pass)+ "_" + str(a_hole) + ".csv"
          df_new=pd.read_csv(file_new)

          if not df_new.empty:
            if a_target == "opticsDS":
              df_new_sampled = df_new.sample(frac=0.62, random_state=42)
            elif a_target == "opticsMS":
              df_new_sampled = df_new.sample(frac=0.785, random_state=42)
            elif a_target == "opticsMD":
              df_new_sampled = df_new.sample(frac=0.71, random_state=42)
            elif a_target == "opticsUM":
              df_new_sampled = df_new.sample(frac=0.88, random_state=42)
            else:
              df_new_sampled = df_new.sample(frac=0.993, random_state=42)

            all_df = pd.concat([all_df,df_new_sampled],axis=0, ignore_index=True)

    if i==0:

      d = {}
      d = {name: pd.DataFrame() for name in all_target[0]}

      for a_target in all_target[0]:
        for a_pass in all_pass[0]:
          for a_hole in all_hole[0]:
            file_new = "csv_output/" + str(a_target) + "_" + str(a_pass) + "/C12_" + str(a_target) + "_" + str(a_pass)+ "_" + str(a_hole) + ".csv"
            df_new=pd.read_csv(file_new)
            if not df_new.empty:
              d[a_target] = pd.concat([d[a_target],df_new],axis=0, ignore_index=True)

      X=all_df.iloc[:,[all_df.columns.get_loc('gem_r'),all_df.columns.get_loc('r_prime')]].values
      y=all_df.iloc[:,[all_df.columns.get_loc('tg_th')]].values
      par_theta = PolynomialRegression(X, y, fitDeg[i], varNms[i])

      x_values=np.array([-512.45, -480, -450, -420, -387.55]) # Location of the C Foils

      means = []

      for a_target in all_target[i]:

        d[a_target]['theta_pred'] = par_theta[0] + par_theta[1]*d[a_target].gem_r + par_theta[2]*d[a_target].r_prime + par_theta[3]*d[a_target].gem_r*d[a_target].gem_r + par_theta[4]*d[a_target].gem_r*d[a_target].r_prime + par_theta[5]*d[a_target].r_prime*d[a_target].r_prime
        d[a_target]['theta_residuals'] = d[a_target]['tg_th'] - d[a_target]['theta_pred']

        hist, bin_edges = np.histogram(d[a_target].theta_residuals, bins=100, density=True)
        bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
        initial_guess = [np.mean(d[a_target].theta_residuals), np.std(d[a_target].theta_residuals)]
        params, covariance = curve_fit(gaussian, bin_centers, hist, p0=initial_guess)
        mu, sigma = params
        means.append(mu*1000)

      color = cmap(0)

      coefs = np.polyfit(x_values, means, 2)  # Fit a second-degree polynomial
      plt.scatter(x_values, means, label=f'Iteration 0 Means', s=50, color=color)

      print(coefs)

      poly = np.poly1d(coefs)

      x_fit = np.linspace(min(x_values), max(x_values), 100)
      y_fit = poly(x_fit)
      plt.plot(x_fit, y_fit, linewidth=2, color=color)
      
      for itr in range(1):

        new_means = []
        color = cmap(itr+1)

        for a_target in all_target[i]:

          d[a_target]['residual_correction'] = (coefs[0]*(d[a_target].tg_vz/10.)*(d[a_target].tg_vz/10.) + coefs[1]*d[a_target].tg_vz/10. + coefs[2])/1000
          d[a_target]['theta_pred'] = d[a_target]['theta_pred'] + d[a_target]['residual_correction']
          d[a_target]['theta_residuals'] = d[a_target]['tg_th'] - d[a_target]['theta_pred']

          hist, bin_edges = np.histogram(d[a_target].theta_residuals, bins=100, density=True)
          bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
          initial_guess = [np.mean(d[a_target].theta_residuals), np.std(d[a_target].theta_residuals)]
          params, covariance = curve_fit(gaussian, bin_centers, hist, p0=initial_guess)
          mu, sigma = params
          new_means.append(mu*1000)

        coefs = np.polyfit(x_values, new_means, 2)
        plt.scatter(x_values, new_means, label=f'Iteration {itr+1} Means', s=50,color=color)
        print(itr+1, coefs)

        poly = np.poly1d(coefs)

        x_fit = np.linspace(min(x_values), max(x_values), 100)
        y_fit = poly(x_fit)
        plt.plot(x_fit, y_fit, linewidth=2,color=color)

      plt.xlim(min(x_values) - 10, max(x_values) + 10)
      plt.ylim(-1, 1)
      plt.xlabel('$v_{z}$ wrt the Global Origin [cm]', fontsize=14)
      plt.ylabel('Residual Means (mrad)', fontsize=14)
      plt.title('Means from Multiple Iterations with Polynomial Fits')
      plt.grid(True)
      plt.legend()
      plt.show()

      all_df_new = pd.DataFrame()

      for a_target in all_target[i]:
        all_df_new = pd.concat([all_df_new,d[a_target]],axis=0, ignore_index=True)

      hist, bin_edges = np.histogram(all_df_new.theta_residuals*1000, bins=100, density=True)
      bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
      initial_guess = [np.mean(all_df_new.theta_residuals*1000), np.std(all_df_new.theta_residuals*1000)]
      params, covariance = curve_fit(gaussian, bin_centers, hist, p0=initial_guess)
      mu, sigma = params

      x = np.linspace(min(bin_centers), max(bin_centers), 100)
      fitted_curve = gaussian(x, mu, sigma)
      plt.hist(all_df_new.theta_residuals*1000, bins=100, density=True, alpha=0.7, label='Histogram')
      plt.plot(x, fitted_curve, 'r-', label='Fitted Gaussian')
      plt.xlim(-10,10)
      plt.xlabel(r'$\theta_{lab}^{True} - \theta_{lab}^{Pred}$ [mrad]', fontsize=14)
      plt.ylabel('Counts [Arbitrary Units]', fontsize=14)
      plt.legend()
      plt.text(0.05, 0.9, f'Mean = {mu:.6f} mrad', transform=plt.gca().transAxes, fontsize=12, color='blue')
      plt.text(0.05, 0.85, f'St. Dev. = {sigma:.6f} mrad', transform=plt.gca().transAxes, fontsize=12, color='blue')
      plt.show()

    else:

      X=all_df.iloc[:,[all_df.columns.get_loc('gem_r'), all_df.columns.get_loc('r_prime'), all_df.columns.get_loc('gem_ph'),all_df.columns.get_loc('phi_prime')]].values
      y=all_df.iloc[:,[all_df.columns.get_loc('tg_ph')]].values
      par_theta = PolynomialRegression(X, y, fitDeg[i], varNms[i])


    
